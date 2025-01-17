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
#include "gpg/GpgSubKey.h"

GpgSubKey::GpgSubKey(gpgme_subkey_t key) {

    if (key == nullptr) return;

    id = key->keyid;
    pubkey_algo = gpgme_pubkey_algo_name(key->pubkey_algo);
    fpr = key->fpr;

    expired = key->expired;
    revoked = key->revoked;
    secret = key->secret;

    disabled = key->disabled;

    length = key->length;

    can_authenticate = key->can_authenticate;
    can_certify = key->can_certify;
    can_encrypt = key->can_encrypt;
    can_sign = key->can_sign;
    is_cardkey = key->is_cardkey;
    is_private_key = key->secret;

    timestamp = QDateTime::fromTime_t(key->timestamp);
    expires = QDateTime::fromTime_t(key->expires);
}
