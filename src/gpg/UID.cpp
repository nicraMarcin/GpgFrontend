//
// Created by eric on 2021/5/22.
//

#include "gpg/UID.h"

UID::UID(gpgme_user_id_t user_id) :
        uid(user_id->uid), name(user_id->name), email(user_id->email), comment(user_id->comment) {

    auto sig = user_id->signatures;

    while (sig != nullptr) {
        signatures.push_back(Signature(sig));
    }

}