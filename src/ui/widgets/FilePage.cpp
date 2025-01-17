/**
 * This file is part of GPGFrontend.
 *
 * GPGFrontend is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 *
 * The initial version of the source code is inherited from gpg4usb-team.
 * Their source code version also complies with GNU General Public License.
 *
 * The source code version of this software was modified and released
 * by Saturneric<eric@bktus.com> starting on May 12, 2021.
 *
 */

#include "ui/widgets/FilePage.h"

#include "MainWindow.h"

FilePage::FilePage(QWidget *parent) : QWidget(parent) {

    qDebug() << "First Parent" << parent;
    firstParent = parent;

    qDebug() << "New File Page";

    dirModel = new QFileSystemModel();
    dirModel->setRootPath(QDir::currentPath());

    dirTreeView = new QTreeView();
    dirTreeView->setModel(dirModel);
    dirTreeView->setAnimated(true);
    dirTreeView->setIndentation(20);
    dirTreeView->setRootIndex(dirModel->index(QDir::currentPath()));
    dirTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    mPath = dirModel->rootPath();

    createPopupMenu();


    upLevelButton = new QPushButton();
    connect(upLevelButton, SIGNAL(clicked(bool)), this, SLOT(slotUpLevel()));

    QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";


    auto upPixmap = QPixmap(":up.png");
    upPixmap = upPixmap.scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QIcon upButtonIcon(upPixmap);
    upLevelButton->setIcon(upButtonIcon);
    upLevelButton->setIconSize(upPixmap.rect().size());
    upLevelButton->setStyleSheet(buttonStyle);

    refreshButton = new QPushButton("Refresh");
    connect(refreshButton, SIGNAL(clicked(bool)), this, SLOT(slotGoPath()));

    goPathButton = new QPushButton();
    connect(goPathButton, SIGNAL(clicked(bool)), this, SLOT(slotGoPath()));

    auto updatePixmap = QPixmap(":refresh.png");
    updatePixmap = updatePixmap.scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QIcon updateButtonIcon(updatePixmap);
    goPathButton->setIcon(updateButtonIcon);
    goPathButton->setIconSize(updatePixmap.rect().size());
    goPathButton->setStyleSheet(buttonStyle);

    pathEdit = new QLineEdit();
    pathEdit->setText(dirModel->rootPath());

    auto *menuLayout = new QHBoxLayout();
    menuLayout->addWidget(upLevelButton);
    menuLayout->setStretchFactor(upLevelButton, 1);
    menuLayout->addWidget(pathEdit);
    menuLayout->setStretchFactor(pathEdit, 10);
    menuLayout->addWidget(goPathButton);
    menuLayout->setStretchFactor(goPathButton, 1);
    // menuLayout->addWidget(refreshButton);
    // menuLayout->setStretchFactor(refreshButton, 1);

    auto *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(menuLayout);
    layout->setStretchFactor(menuLayout, 1);
    layout->addWidget(dirTreeView);
    layout->setStretchFactor(dirTreeView, 8);

    this->setLayout(layout);

    connect(dirTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(fileTreeViewItemClicked(const QModelIndex &)));
    connect(dirTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(fileTreeViewItemDoubleClicked(const QModelIndex &)));
    connect(dirTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    emit pathChanged(mPath);

}

void FilePage::fileTreeViewItemClicked(const QModelIndex &index) {
    mPath = dirModel->fileInfo(index).absoluteFilePath();
    qDebug() << "mPath" << mPath;
}

void FilePage::slotUpLevel() {
    QModelIndex currentRoot = dirTreeView->rootIndex();
    mPath = dirModel->fileInfo(currentRoot.parent()).absoluteFilePath();
    auto fileInfo = QFileInfo(mPath);
    if(fileInfo.isDir() && fileInfo.isReadable() && fileInfo.isExecutable()) {
        dirTreeView->setRootIndex(currentRoot.parent());
        pathEdit->setText(mPath);
    }
    qDebug() << "Current Root mPath" << mPath;
    emit pathChanged(mPath);
}

void FilePage::fileTreeViewItemDoubleClicked(const QModelIndex &index) {
    mPath = dirModel->fileInfo(index).absoluteFilePath();
    auto fileInfo = QFileInfo(mPath);
    if(fileInfo.isDir() && fileInfo.isReadable() && fileInfo.isExecutable()) {
        dirTreeView->setRootIndex(index);
        pathEdit->setText(mPath);
    }
    qDebug() << "Index mPath" << mPath;
    emit pathChanged(mPath);
}

QString FilePage::getSelected() const {
    return mPath;
}

void FilePage::slotGoPath() {
    qDebug() << "getSelected" << pathEdit->text();
    auto fileInfo = QFileInfo(pathEdit->text());
    if(fileInfo.isDir() && fileInfo.isReadable() && fileInfo.isExecutable()) {
        mPath = fileInfo.filePath();
        qDebug() << "Set Path" << mPath;
        dirTreeView->setRootIndex(dirModel->index(fileInfo.filePath()));
    } else {
        QMessageBox::critical(this, "Error", "The path is unprivileged or unreachable.");
    }
    emit pathChanged(mPath);
}

void FilePage::createPopupMenu() {
    popUpMenu = new QMenu();

    auto openItemAct = new QAction(tr("Open"), this);
    connect(openItemAct, SIGNAL(triggered()), this, SLOT(slotOpenItem()));
    auto deleteItemAct = new QAction(tr("Delete"), this);
    connect(deleteItemAct, SIGNAL(triggered()), this, SLOT(slotDeleteItem()));
    encryptItemAct = new QAction(tr("Encrypt and Sign"), this);
    connect(encryptItemAct, SIGNAL(triggered()), this, SLOT(slotEncryptItem()));
    decryptItemAct = new QAction(tr("Decrypt and Verify"), this);
    connect(decryptItemAct, SIGNAL(triggered()), this, SLOT(slotDecryptItem()));
    signItemAct = new QAction(tr("Only Sign"), this);
    connect(signItemAct, SIGNAL(triggered()), this, SLOT(slotSignItem()));
    verifyItemAct = new QAction(tr("Only Verify"), this);
    connect(verifyItemAct, SIGNAL(triggered()), this, SLOT(slotVerifyItem()));

    popUpMenu->addAction(openItemAct);
    popUpMenu->addAction(deleteItemAct);
    popUpMenu->addSeparator();
    popUpMenu->addAction(encryptItemAct);
    popUpMenu->addAction(decryptItemAct);
    popUpMenu->addAction(signItemAct);
    popUpMenu->addAction(verifyItemAct);

}

void FilePage::onCustomContextMenu(const QPoint &point) {
    QModelIndex index = dirTreeView->indexAt(point);
    mPath = dirModel->fileInfo(index).absoluteFilePath();
    qDebug() << "Right Click" <<  mPath;
    if (index.isValid()) {
        QFileInfo info(mPath);
        encryptItemAct->setEnabled(info.isFile() && (info.suffix() != "gpg" && info.suffix() != "sig"));
        decryptItemAct->setEnabled(info.isFile() && info.suffix() == "gpg");
        signItemAct->setEnabled(info.isFile() && (info.suffix() != "gpg" && info.suffix() != "sig"));
        verifyItemAct->setEnabled(info.isFile() && (info.suffix() == "sig" || info.suffix() == "gpg"));

        popUpMenu->exec(dirTreeView->viewport()->mapToGlobal(point));
    }
}

void FilePage::slotOpenItem() {
    QFileInfo info(mPath);
    if(info.isDir()) {
        qDebug() << "getSelected" << pathEdit->text();
        if(info.isReadable() && info.isExecutable()) {
            qDebug() << "Set Path" << info.filePath();
            dirTreeView->setRootIndex(dirModel->index(info.filePath()));
        } else {
            QMessageBox::critical(this, "Error", "The path is unprivileged or unreachable.");
        }
    } else {
        auto mainWindow = qobject_cast<MainWindow *>(firstParent);
        qDebug() << "Open Item" << mPath;
        if (mainWindow != nullptr)
            mainWindow->slotOpenFile(mPath);
    }
}

void FilePage::slotDeleteItem() {
    QModelIndex index = dirTreeView->currentIndex();
    QVariant data = dirTreeView->model()->data(index);

    auto ret = QMessageBox::warning(this,
                                    tr("Warning"),
                                    tr("Are you sure you want to delete it?"),
                                    QMessageBox::Ok | QMessageBox::Cancel);

    if(ret == QMessageBox::Cancel)
        return;

    qDebug() << "Delete Item" << data.toString();

    if(!dirModel->remove(index)){
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Unable to delete the file or folder."));
    }
}

void FilePage::slotEncryptItem() {
    auto mainWindow = qobject_cast<MainWindow *>(firstParent);
    if(mainWindow != nullptr)
        mainWindow->slotFileEncryptSign();
}

void FilePage::slotDecryptItem() {
    auto mainWindow = qobject_cast<MainWindow *>(firstParent);
    if(mainWindow != nullptr)
        mainWindow->slotFileDecryptVerify();
}

void FilePage::slotSignItem() {
    auto mainWindow = qobject_cast<MainWindow *>(firstParent);
    if(mainWindow != nullptr)
        mainWindow->slotFileSign();
}

void FilePage::slotVerifyItem() {
    auto mainWindow = qobject_cast<MainWindow *>(firstParent);
    if(mainWindow != nullptr)
        mainWindow->slotFileVerify();
}
