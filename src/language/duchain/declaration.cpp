/* This file is part of KDevelop
    Copyright (C) 2006 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "declaration.h"

#include <ktexteditor/smartrange.h>
#include <ktexteditor/document.h>

#include "topducontext.h"
#include "use.h"
#include "definition.h"
#include "symboltable.h"
#include "forwarddeclaration.h"
#include "duchain.h"
#include "duchainlock.h"
#include "identifiedtype.h"

using namespace KTextEditor;

namespace KDevelop
{

class DeclarationPrivate
{
public:
  DUContext* m_context;
  Declaration::Scope m_scope;
  AbstractType::Ptr m_type;
  Identifier m_identifier;

  QList<ForwardDeclaration*> m_forwardDeclarations;

  Definition* m_definition;

  QList<Use*> m_uses;

  bool m_isDefinition  : 1;
  bool m_inSymbolTable : 1;
};

Declaration::Declaration(KTextEditor::Range* range, Scope scope, DUContext* context )
  : DUChainBase(range)
  , d(new DeclarationPrivate)
{
  d->m_context = 0;
  d->m_scope = scope;
  d->m_definition = 0;
  d->m_isDefinition = false;
  d->m_inSymbolTable = false;
  Q_ASSERT(context);
  setContext(context);
}

Declaration::~Declaration()
{
  // Inserted by the builder after construction has finished.
  if (d->m_inSymbolTable)
    SymbolTable::self()->removeDeclaration(this);

  foreach (ForwardDeclaration* forward, d->m_forwardDeclarations)
    forward->setResolved(0);
  Q_ASSERT(d->m_forwardDeclarations.isEmpty());

  QList<Use*> _uses = uses();
  foreach (Use* use, _uses)
    use->setDeclaration(0);

  if (Definition* def = definition())
    def->setDeclaration(0);

  // context is only null in the test cases
  if (context())
    context()->removeDeclaration(this);

  setContext(0);

  setAbstractType(AbstractType::Ptr());

  DUChain::declarationChanged(this, DUChainObserver::Deletion, DUChainObserver::NotApplicable);
  delete d;
}

void Declaration::removeUse( Use* use )
{
  ENSURE_CHAIN_WRITE_LOCKED

  use->setDeclaration(0L);
  d->m_uses.removeAll(use);

  DUChain::declarationChanged(this, DUChainObserver::Removal, DUChainObserver::Uses, use);
}

void Declaration::addUse( Use* use )
{
  ENSURE_CHAIN_WRITE_LOCKED

  use->setDeclaration(this);
  d->m_uses.append(use);

  DUChain::declarationChanged(this, DUChainObserver::Addition, DUChainObserver::Uses, use);
}

const QList< Use* > & Declaration::uses( ) const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_uses;
}

const Identifier& Declaration::identifier( ) const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_identifier;
}

void Declaration::setIdentifier(const Identifier& identifier)
{
  ENSURE_CHAIN_WRITE_LOCKED

  d->m_identifier = identifier;

  DUChain::declarationChanged(this, DUChainObserver::Change, DUChainObserver::Identifier);
}

AbstractType::Ptr Declaration::abstractType( ) const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_type;
}

void Declaration::setAbstractType(AbstractType::Ptr type)
{
  ENSURE_CHAIN_WRITE_LOCKED

  if (IdentifiedType* idType = dynamic_cast<IdentifiedType*>(d->m_type.data()))
    idType->setDeclaration(0);

  if (d->m_type)
    DUChain::declarationChanged(this, DUChainObserver::Removal, DUChainObserver::DataType);

  d->m_type = type;

  if (IdentifiedType* idType = dynamic_cast<IdentifiedType*>(d->m_type.data()))
    idType->setDeclaration(this);

  if (d->m_type)
    DUChain::declarationChanged(this, DUChainObserver::Addition, DUChainObserver::DataType);
}

Declaration::Scope Declaration::scope( ) const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_scope;
}

QualifiedIdentifier Declaration::qualifiedIdentifier() const
{
  ENSURE_CHAIN_READ_LOCKED

  QualifiedIdentifier ret = context()->scopeIdentifier(true);
  ret.push(identifier());
  return ret;
}

QString Declaration::mangledIdentifier() const
{
  //GNU mangling specs from http://theory.uwinnipeg.ca/gnu/gcc/gxxint_15.html

  if (abstractType())
    return abstractType()->mangled();

  // Error...
  return qualifiedIdentifier().mangled();
}

DUContext * Declaration::context() const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_context;
}

void Declaration::setContext(DUContext* context)
{
  ENSURE_CHAIN_WRITE_LOCKED

  if (d->m_context && context)
    Q_ASSERT(d->m_context->topContext() == context->topContext());

  if (d->m_context) {
    d->m_context->removeDeclaration(this);
    DUChain::declarationChanged(this, DUChainObserver::Removal, DUChainObserver::Context, d->m_context);
  }

  d->m_context = context;

  if (d->m_context) {
    d->m_context->addDeclaration(this);
    DUChain::declarationChanged(this, DUChainObserver::Addition, DUChainObserver::Context, d->m_context);
  }
}

bool Declaration::operator ==(const Declaration & other) const
{
  ENSURE_CHAIN_READ_LOCKED

  return this == &other;
}

QString Declaration::toString() const
{
  return QString("Declaration: %3 %4").arg(abstractType() ? abstractType()->toString() : QString("<notype>")).arg(identifier().toString());
}

// kate: indent-width 2;

bool Declaration::isDefinition() const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_isDefinition;
}

void Declaration::setDeclarationIsDefinition(bool dd)
{
  ENSURE_CHAIN_WRITE_LOCKED

  d->m_isDefinition = dd;
  if (d->m_isDefinition && definition()) {
    setDefinition(0);
  }
}

Definition* Declaration::definition() const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_definition;
}

void Declaration::setDefinition(Definition* definition)
{
  ENSURE_CHAIN_WRITE_LOCKED

  if (d->m_definition) {
    d->m_definition->setDeclaration(0);

    DUChain::declarationChanged(this, DUChainObserver::Removal, DUChainObserver::DefinitionRelationship, d->m_definition);
  }

  d->m_definition = definition;

  if (d->m_definition) {
    d->m_definition->setDeclaration(this);
    d->m_isDefinition = false;

    DUChain::declarationChanged(this, DUChainObserver::Addition, DUChainObserver::DefinitionRelationship, d->m_definition);
  }
}

bool Declaration::inSymbolTable() const
{
  return d->m_inSymbolTable;
}

void Declaration::setInSymbolTable(bool inSymbolTable)
{
  d->m_inSymbolTable = inSymbolTable;
}

const QList< ForwardDeclaration * > & Declaration::forwardDeclarations() const
{
  ENSURE_CHAIN_READ_LOCKED

  return d->m_forwardDeclarations;
}

void Declaration::addForwardDeclaration( ForwardDeclaration* declaration)
{
  ENSURE_CHAIN_WRITE_LOCKED

  d->m_forwardDeclarations.append( declaration );
}

void Declaration::removeForwardDeclaration( ForwardDeclaration* declaration)
{
  ENSURE_CHAIN_WRITE_LOCKED

  d->m_forwardDeclarations.removeAll( declaration );
}

bool Declaration::isForwardDeclaration() const
{
  return false;
}

ForwardDeclaration* Declaration::toForwardDeclaration()
{
  return static_cast<ForwardDeclaration*>(this);
}

const ForwardDeclaration* Declaration::toForwardDeclaration() const
{
  return static_cast<const ForwardDeclaration*>(this);
}

TopDUContext * Declaration::topContext() const
{
  if (context())
    return context()->topContext();

  return 0;
}
}

// kate: space-indent on; indent-width 2; tab-width: 4; replace-tabs on; auto-insert-doxygen on
