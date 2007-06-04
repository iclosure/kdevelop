/* This file is part of the KDevelop project
Copyright (C) 2002 F@lk Brettschneider <falkbr@kdevelop.org>
Copyright (C) 2003 John Firebaugh <jfirebaugh@kde.org>
Copyright (C) 2006 Adam Treat <treat@kde.org>
Copyright (C) 2006, 2007 Alexander Dymo <adymo@kdevelop.org>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/
namespace KDevelop {

void MainWindowPrivate::gotoNextWindow()
{
    if ( ( center->currentIndex() + 1 ) < center->count() )
        center->setCurrentIndex( center->currentIndex() + 1 );
    else
        center->setCurrentIndex( 0 );
}

void MainWindowPrivate::gotoPreviousWindow()
{
    if ( ( center->currentIndex() - 1 ) >= 0 )
        center->setCurrentIndex( center->currentIndex() - 1 );
    else
        center->setCurrentIndex( center->count() - 1 );
}

void MainWindowPrivate::gotoFirstWindow()
{
    center->setCurrentIndex( 0 );
}

void MainWindowPrivate::gotoLastWindow()
{
    center->setCurrentIndex( center->count() - 1 );
}

void MainWindowPrivate::projectOpened()
{
    QString app = i18n( "Project" );
    QString text = i18n( "Configure %1", app );
    QAction *action = actionCollection() ->action( "settings_configure" );
    action->setToolTip( text );
    action->setWhatsThis( QString( "<b>%1</b><p>%2" ).arg( text ).arg(
                              i18n( "Lets you customize %1.", app ) ) );
}

void MainWindowPrivate::projectClosed()
{
    QString app = qApp->applicationName();
    QString text = i18n( "Configure %1", app );
    QAction *action = actionCollection() ->action( "settings_configure" );
    action->setToolTip( text );
    action->setWhatsThis( QString( "<b>%1</b><p>%2" ).arg( text ).arg(
                              i18n( "Lets you customize %1.", app ) ) );
}

void MainWindowPrivate::configureToolbars()
{}

void MainWindowPrivate::newToolbarConfig()
{
    m_mainWindow->applyMainWindowSettings( KConfigGroup(KGlobal::config(), "MainWindow") );
}

void MainWindowPrivate::toggleStatusbar()
{
    KToggleAction * action =
        qobject_cast< KToggleAction*>( actionCollection() ->action( "settings_show_statusbar" ) );
    m_mainWindow->statusBar()->setHidden( !action->isChecked() );
}

void MainWindowPrivate::stopButtonPressed()
{}

void MainWindowPrivate::stopPopupActivated( int id )
{
    Q_UNUSED( id );
}

void MainWindowPrivate::stopMenuAboutToShow()
{}

void MainWindowPrivate::showMenuBar()
{}

void MainWindowPrivate::configureNotifications()
{
    KNotifyConfigWidget::configure( m_mainWindow, "Notification Configuration Dialog" );
}

void MainWindowPrivate::settingsDialog()
{
    Core::self()->uiControllerInternal()->showSettingsDialog();
}

void MainWindowPrivate::configureEditors()
{
    //FIXME Change this so that it is embedded in our config dialog.
    //Perhaps this will require a change to the KTextEditor interface too...
/*    KTextEditor::Document * doc =
        Core::documentController() ->activeDocument() ->textDocument();
    KTextEditor::Editor *editor = doc ? doc->editor() : 0;
    if ( !editor )
    {
        return ;
    }

    if ( !editor->configDialogSupported() )
    {
        kDebug( 9000 ) << "KTextEditor::configDialogSupported() == false" << endl;
    }

    // show the modal config dialog for this part if it has a ConfigInterface
    editor->configDialog( this );*/
}

void MainWindowPrivate::keyBindings()
{}

void MainWindowPrivate::newWindow()
{
    Core::self()->uiControllerInternal()->switchToArea(m_mainWindow->area()->objectName(), UiController::NewWindow);
}

void MainWindowPrivate::splitHorizontal()
{
    split(Qt::Vertical);
}

void MainWindowPrivate::splitVertical()
{
    split(Qt::Horizontal);
}

void MainWindowPrivate::split(Qt::Orientation orientation)
{
    if (!m_mainWindow->area())
        return;
    Sublime::View *view = m_mainWindow->activeView();
    if (!view)
        return;

    Sublime::View *newView = view->document()->createView();
    m_mainWindow->area()->addView(newView, view, orientation);
    m_mainWindow->activateView(newView);
}

void MainWindowPrivate::fileNew()
{
    Core::self()->uiControllerInternal()->openEmptyDocument();
}

void MainWindowPrivate::fileClose()
{
    Sublime::View *view = m_mainWindow->activeView();
    if (!view)
        return;
    m_mainWindow->area()->removeView(view);
}

void MainWindowPrivate::viewAddNewToolView()
{
    Core::self()->uiControllerInternal()->addNewToolView(m_mainWindow);
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
