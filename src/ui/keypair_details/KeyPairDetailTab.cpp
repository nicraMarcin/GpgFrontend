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
 * by Saturneric<eric@bktus.com><eric@bktus.com> starting on May 12, 2021.
 *
 */

#include "ui/keypair_details/KeyPairDetailTab.h"

KeyPairDetailTab::KeyPairDetailTab(GpgME::GpgContext *ctx, const GpgKey &mKey, QWidget *parent) : mKey(mKey), QWidget(parent) {

    mCtx = ctx;
    keyid = new QString(mKey.id);

    ownerBox = new QGroupBox(tr("Owner"));
    keyBox = new QGroupBox(tr("Master Key"));
    fingerprintBox = new QGroupBox(tr("Fingerprint"));
    additionalUidBox = new QGroupBox(tr("Additional UIDs"));

    nameVarLabel = new QLabel();
    nameVarLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    emailVarLabel = new QLabel();
    emailVarLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    commentVarLabel = new QLabel();
    commentVarLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    keyidVarLabel = new QLabel();
    keyidVarLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    usageVarLabel = new QLabel();
    actualUsageVarLabel = new QLabel();

    keySizeVarLabel = new QLabel();
    expireVarLabel = new QLabel();
    createdVarLabel = new QLabel();
    algorithmVarLabel = new QLabel();

    // Show the situation that master key not exists.
    masterKeyExistVarLabel = new QLabel(mKey.has_master_key ? "Exists" : "Not Exists");
    if(!mKey.has_master_key){
        auto paletteExpired = masterKeyExistVarLabel->palette();
        paletteExpired.setColor(masterKeyExistVarLabel->foregroundRole(), Qt::red);
        masterKeyExistVarLabel->setPalette(paletteExpired);
    } else {
        auto paletteValid = masterKeyExistVarLabel->palette();
        paletteValid.setColor(masterKeyExistVarLabel->foregroundRole(), Qt::darkGreen);
        masterKeyExistVarLabel->setPalette(paletteValid);
    }

    if(mKey.expired){
        auto paletteExpired = expireVarLabel->palette();
        paletteExpired.setColor(expireVarLabel->foregroundRole(), Qt::red);
        expireVarLabel->setPalette(paletteExpired);
    } else {
        auto paletteValid = expireVarLabel->palette();
        paletteValid.setColor(expireVarLabel->foregroundRole(), Qt::darkGreen);
        expireVarLabel->setPalette(paletteValid);
    }

    auto *mvbox = new QVBoxLayout();
    auto *vboxKD = new QGridLayout();
    auto *vboxOD = new QGridLayout();

    vboxOD->addWidget(new QLabel(tr("Name:")), 0, 0);
    vboxOD->addWidget(new QLabel(tr("Email Address:")), 1, 0);
    vboxOD->addWidget(new QLabel(tr("Comment:")), 2, 0);
    vboxOD->addWidget(nameVarLabel, 0, 1);
    vboxOD->addWidget(emailVarLabel, 1, 1);
    vboxOD->addWidget(commentVarLabel, 2, 1);

    vboxKD->addWidget(new QLabel(tr("Key ID: ")), 0, 0);
    vboxKD->addWidget(new QLabel(tr("Algorithm: ")), 1, 0);
    vboxKD->addWidget(new QLabel(tr("Key Size:")), 2, 0);
    vboxKD->addWidget(new QLabel(tr("Nominal Usage: ")), 3, 0);
    vboxKD->addWidget(new QLabel(tr("Actual Usage: ")), 4, 0);
    vboxKD->addWidget(new QLabel(tr("Expires on: ")), 5, 0);
    vboxKD->addWidget(new QLabel(tr("Last Update: ")), 6, 0);
    vboxKD->addWidget(new QLabel(tr("Secret Key Existence: ")), 7, 0);


    vboxKD->addWidget(keySizeVarLabel, 2, 1);
    vboxKD->addWidget(expireVarLabel, 5, 1);
    vboxKD->addWidget(algorithmVarLabel, 1, 1);
    vboxKD->addWidget(createdVarLabel, 6, 1);
    vboxKD->addWidget(keyidVarLabel, 0, 1);
    vboxKD->addWidget(usageVarLabel, 3, 1);
    vboxKD->addWidget(actualUsageVarLabel, 4, 1);
    vboxKD->addWidget(masterKeyExistVarLabel, 7, 1);

    ownerBox->setLayout(vboxOD);
    mvbox->addWidget(ownerBox);
    keyBox->setLayout(vboxKD);
    mvbox->addWidget(keyBox);

    fingerPrintVarLabel = new QLabel(beautifyFingerprint(mKey.fpr));
    fingerPrintVarLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    fingerPrintVarLabel->setStyleSheet("margin-left: 0; margin-right: 5;");
    auto *hboxFP = new QHBoxLayout();

    hboxFP->addWidget(fingerPrintVarLabel);

    auto *copyFingerprintButton = new QPushButton(tr("Copy"));
    copyFingerprintButton->setFlat(true);
    copyFingerprintButton->setToolTip(tr("copy fingerprint to clipboard"));
    connect(copyFingerprintButton, SIGNAL(clicked()), this, SLOT(slotCopyFingerprint()));

    hboxFP->addWidget(copyFingerprintButton);

    fingerprintBox->setLayout(hboxFP);
    mvbox->addWidget(fingerprintBox);
    mvbox->addStretch();

    if (mKey.is_private_key) {
        auto *privKeyBox = new QGroupBox(tr("Operations"));
        auto *vboxPK = new QVBoxLayout();

        auto *exportButton = new QPushButton(tr("Export Private Key (Include Subkeys)"));
        vboxPK->addWidget(exportButton);
        connect(exportButton, SIGNAL(clicked()), this, SLOT(slotExportPrivateKey()));

        if(mKey.has_master_key) {
            auto *editExpiresButton = new QPushButton(tr("Modify Expiration Datetime (Master Key)"));
            vboxPK->addWidget(editExpiresButton);
            connect(editExpiresButton, SIGNAL(clicked()), this, SLOT(slotModifyEditDatetime()));

            auto *keyServerOperaButton = new QPushButton(tr("Key Server Operation (Pubkey)"));
            keyServerOperaButton->setStyleSheet("text-align:center;");
            vboxPK->addWidget(keyServerOperaButton);
            connect(keyServerOperaButton, SIGNAL(clicked()), this, SLOT(slotModifyEditDatetime()));

            // Set Menu
            createKeyServerOperaMenu();
            keyServerOperaButton->setMenu(keyServerOperaMenu);
        }

        privKeyBox->setLayout(vboxPK);
        mvbox->addWidget(privKeyBox);
    }

    if ((mKey.expired) || (mKey.revoked)) {
        auto *expBox = new QHBoxLayout();
        QPixmap pixmap(":warning.png");

        auto *expLabel = new QLabel();
        auto *iconLabel = new QLabel();
        if (mKey.expired) {
            expLabel->setText(tr("Warning: The Master Key has expired."));
        }
        if (mKey.revoked) {
            expLabel->setText(tr("Warning: The Master Key has been revoked"));
        }

        iconLabel->setPixmap(pixmap.scaled(24,24,Qt::KeepAspectRatio));
        QFont font = expLabel->font();
        font.setBold(true);
        expLabel->setFont(font);
        expLabel->setAlignment(Qt::AlignCenter);
        expBox->addWidget(iconLabel);
        expBox->addWidget(expLabel);
        mvbox->addLayout(expBox);
    }

    mvbox->setContentsMargins(0, 0, 0, 0);

    connect(mCtx, SIGNAL(signalKeyInfoChanged()), this, SLOT(slotRefreshKeyInfo()));

    slotRefreshKeyInfo();
    setAttribute(Qt::WA_DeleteOnClose, true);
    setLayout(mvbox);
}

void KeyPairDetailTab::slotExportPrivateKey() {
    // Show a information box with explanation about private key
    int ret = QMessageBox::information(this, tr("Exporting private Key"),
                                       tr("<h3>You are about to export your <font color=\"red\">PRIVATE KEY</font>!</h3>\n"
                                          "This is NOT your Public Key, so DON'T give it away.<br />"
                                          "Do you REALLY want to export your PRIVATE KEY?"),
                                       QMessageBox::Cancel | QMessageBox::Ok);

    // export key, if ok was clicked
    if (ret == QMessageBox::Ok) {
        auto *keyArray = new QByteArray();

    if(!mCtx->exportSecretKey(mKey, keyArray)) {
         QMessageBox::critical(this, "Error", "An error occurred during the export operation.");
         return;
    }

        auto &key = mCtx->getKeyById(*keyid);
        QString fileString = key.name + " " +key.email + "(" +
                             key.id + ")_secret.asc";
        QString fileName = QFileDialog::getSaveFileName(this, tr("Export Key To File"), fileString,
                                                        tr("Key Files") + " (*.asc *.txt);;All Files (*)");
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(nullptr, tr("Export Error"), tr("Couldn't open %1 for writing").arg(fileName));
            return;
        }
        QTextStream stream(&file);
        stream << *keyArray;
        file.close();
        delete keyArray;
    }
}

QString KeyPairDetailTab::beautifyFingerprint(QString fingerprint) {
    uint len = fingerprint.length();
    if ((len > 0) && (len % 4 == 0))
        for (uint n = 0; 4 * (n + 1) < len; ++n)
            fingerprint.insert(static_cast<int>(5u * n + 4u), ' ');
    return fingerprint;
}

void KeyPairDetailTab::slotCopyFingerprint() {
    QString fpr = fingerPrintVarLabel->text().trimmed().replace(" ", "");
    QClipboard *cb = QApplication::clipboard();
    cb->setText(fpr);
}

void KeyPairDetailTab::slotModifyEditDatetime() {
    auto dialog = new KeySetExpireDateDialog(mCtx, mKey, nullptr, this);
    dialog->show();
}

void KeyPairDetailTab::slotRefreshKeyInfo() {

    nameVarLabel->setText(mKey.name);
    emailVarLabel->setText(mKey.email);

    commentVarLabel->setText(mKey.comment);
    keyidVarLabel->setText(mKey.id);

    QString usage;
    QTextStream usage_steam(&usage);

    if(mKey.can_certify)
        usage_steam << "Cert ";
    if(mKey.can_encrypt)
        usage_steam << "Encr ";
    if(mKey.can_sign)
        usage_steam << "Sign ";
    if(mKey.can_authenticate)
        usage_steam << "Auth ";

    usageVarLabel->setText(usage);

    QString actualUsage;
    QTextStream actual_usage_steam(&actualUsage);

    if(GpgME::GpgContext::checkIfKeyCanCert(mKey))
        actual_usage_steam << "Cert ";
    if(GpgME::GpgContext::checkIfKeyCanEncr(mKey))
        actual_usage_steam << "Encr ";
    if(GpgME::GpgContext::checkIfKeyCanSign(mKey))
        actual_usage_steam << "Sign ";
    if(GpgME::GpgContext::checkIfKeyCanAuth(mKey))
        actual_usage_steam << "Auth ";

    actualUsageVarLabel->setText(actualUsage);

    QString keySizeVal, keyExpireVal, keyCreateTimeVal, keyAlgoVal;

    keySizeVal = QString::number(mKey.length);

    if (mKey.expires.toTime_t() == 0) {
        keyExpireVal = tr("Never Expire");
    } else {
        keyExpireVal = mKey.expires.toString();
    }

    keyAlgoVal = mKey.pubkey_algo;
    keyCreateTimeVal = mKey.create_time.toString();

    keySizeVarLabel->setText(keySizeVal);
    expireVarLabel->setText(keyExpireVal);
    createdVarLabel->setText(keyCreateTimeVal);
    algorithmVarLabel->setText(keyAlgoVal);

    fingerPrintVarLabel->setText(beautifyFingerprint(mKey.fpr));

}

void KeyPairDetailTab::createKeyServerOperaMenu() {
    keyServerOperaMenu = new QMenu(this);

    auto *uploadKeyPair = new QAction(tr("Upload Key Pair"), this);
    connect(uploadKeyPair, SIGNAL(triggered()), this, SLOT(slotUploadKeyToServer()));
    auto *updateKeyPair = new QAction(tr("Update Key Pair"), this);
    connect(updateKeyPair, SIGNAL(triggered()), this, SLOT(slotUpdateKeyToServer()));

    keyServerOperaMenu->addAction(uploadKeyPair);
    // TODO Solve Refresh Problem
//    keyServerOperaMenu->addAction(updateKeyPair);
}

void KeyPairDetailTab::slotUploadKeyToServer() {
    QVector<GpgKey> keys;
    keys.append(mKey);
    auto *dialog = new KeyUploadDialog(mCtx, keys);
}

void KeyPairDetailTab::slotUpdateKeyToServer() {
    QVector<GpgKey> keys;
    keys.append(mKey);
    auto *dialog = new KeyServerImportDialog(mCtx, this);
    dialog->show();
    dialog->slotImportKey(keys);
}

