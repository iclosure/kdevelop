/*
 * KDevelop C++ Code Completion Support
 *
 * Copyright 2007-2008 David Nolden <david.nolden.kdevelop@art-master.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "completionhelpers.h"
#include "completionitem.h"
#include <QList>
#include <QVariant>
#include <language/duchain/declaration.h>
#include <language/duchain/abstractfunctiondeclaration.h>
#include <language/duchain/duchainutils.h>
#include <QTextFormat>
#include <QStringList>
#include "overloadresolution.h"
#include "cpptypes.h"
#include "cppduchain.h"

const int desiredArgumentTypeLength = 20;
const int maxDefaultParameterLength = 30;

using namespace KDevelop;
using namespace Cpp;

void createArgumentList(const NormalDeclarationCompletionItem& item, QString& ret, QList<QVariant>* highlighting )
{
  ///@todo also highlight the matches of the previous arguments, they are given by ViableFunction
  Declaration* dec(item.m_declaration.data());

  Cpp::CodeCompletionContext::Function f;

  if( item.completionContext->memberAccessOperation() == Cpp::CodeCompletionContext::FunctionCallAccess && item.completionContext->functions().count() > item.listOffset )
    f = item.completionContext->functions()[item.listOffset];

  int textFormatStart = 0;
  QTextFormat normalFormat(QTextFormat::CharFormat);
  QTextFormat highlightFormat; //highlightFormat is invalid, so kate uses the match-quality dependent color.
  QTextCharFormat boldFormat;
  boldFormat.setFontWeight(QFont::Bold);

  AbstractFunctionDeclaration* decl = dynamic_cast<AbstractFunctionDeclaration*>(dec);
  FunctionType::Ptr functionType = dec->type<FunctionType>();
  if (functionType && decl) {

    QVector<Declaration*> parameters;
    if( DUChainUtils::getArgumentContext(dec) )
      parameters = DUChainUtils::getArgumentContext(dec)->localDeclarations();

//     QStringList defaultParams = decl->defaultParameters();

    QVector<Declaration*>::const_iterator paramNameIt = parameters.begin();
    uint defaultParamNum = 0;

    int firstDefaultParam = functionType->arguments().count() - decl->defaultParametersSize();

    ret = "(";
    
    if( highlighting && ret.length() != textFormatStart )
    {
      //Add a default-highlighting for the passed text
      *highlighting <<  QVariant(textFormatStart);
      *highlighting << QVariant(ret.length() - textFormatStart);
      *highlighting << boldFormat;
      textFormatStart = ret.length();
    }
    
    bool first = true;
    int num = 0;

    const KDevVarLengthArray<Cpp::ViableFunction::ParameterConversion>& conversions = f.function.parameterConversions();
    uint parameterConversion = 0;

    foreach (const AbstractType::Ptr& argument, functionType->arguments()) {
      if (first)
        first = false;
      else
        ret += ", ";

      bool doHighlight = false;
      QTextFormat doFormat = normalFormat;

      if( ( f.function.isValid() && num == f.matchedArguments ) )
      {
        doHighlight = true;
        doFormat = highlightFormat;

      } else if( num < f.matchedArguments )
      {
        doHighlight = true;
        QTextCharFormat format;

        if( parameterConversion != conversions.size() ) {
          //Interpolate the color
          quint64 badMatchColor = 0xff7777ff; //Full blue
          quint64 goodMatchColor = 0xff77ff77; //Full green

          uint totalColor = (badMatchColor*(Cpp::MaximumConversionResult-conversions[parameterConversion].rank) + goodMatchColor*(conversions[parameterConversion]).rank)/Cpp::MaximumConversionResult;

          format.setUnderlineStyle( QTextCharFormat::WaveUnderline );
          format.setUnderlineColor( QColor(totalColor) );

          ++parameterConversion;
          doFormat = format;
        }
      }

      if( doHighlight )
      {
        if( highlighting && ret.length() != textFormatStart )
        {
          //Add a default-highlighting for the passed text
          *highlighting <<  QVariant(textFormatStart);
          *highlighting << QVariant(ret.length() - textFormatStart);
          *highlighting << QVariant(normalFormat);
          textFormatStart = ret.length();
        }
      }

      if( paramNameIt != parameters.end() /*&& !(*paramNameIt)->identifier().isEmpty()*/ )
        ret += Cpp::shortenedTypeString(*paramNameIt, desiredArgumentTypeLength) + " " + (*paramNameIt)->identifier().toString();
      else if (argument)
        ret += argument->toString();
      else
        ret += "<incomplete type>";

      if( doHighlight  )
      {
        if( highlighting && ret.length() != textFormatStart )
        {
          *highlighting <<  QVariant(textFormatStart);
          *highlighting << QVariant(ret.length() - textFormatStart);
          *highlighting << doFormat;
          textFormatStart = ret.length();
        }
      }

      if( num >= firstDefaultParam ) {
        ret += " = ";
        
        QString defaultParam = decl->defaultParameters()[defaultParamNum].str();
        if(defaultParam.length() <= maxDefaultParameterLength)
          ret += defaultParam;
        else
          ret += "...";
        
        ++defaultParamNum;
      }

      ++num;
      if( paramNameIt != parameters.end() )
        ++paramNameIt;
    }

    if( highlighting && ret.length() != textFormatStart )
    {
      *highlighting <<  QVariant(textFormatStart);
      *highlighting << QVariant(ret.length() - textFormatStart);
      *highlighting << normalFormat;
      textFormatStart = ret.length();
    }
    
    ret += ')';

    if( highlighting && ret.length() != textFormatStart )
    {
      *highlighting <<  QVariant(textFormatStart);
      *highlighting << QVariant(ret.length() - textFormatStart);
      *highlighting << boldFormat;
      textFormatStart = ret.length();
    }

    return;
  }
}


//Returns the type as which a declaration in the completion-list should be interpreted, which especially means that it returns the return-type of a function.
AbstractType::Ptr effectiveType( Declaration* decl )
{
  if( !decl || !decl->abstractType() )
    return AbstractType::Ptr();

  if( decl->type<FunctionType>() )
    return decl->type<FunctionType>()->returnType();

  return decl->abstractType();
}

