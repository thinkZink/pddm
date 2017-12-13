/*
 * CryptoLibrary.h
 *
 *  Created on: May 19, 2016
 *      Author: edward
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <array>

namespace pddm {
namespace messaging {
class MessageBody;
class StringBody;
class OverlayMessage;
struct ValueTuple;
struct ValueContribution;
struct SignedValue;
} /* namespace messaging */
} /* namespace pddm */

namespace pddm {

namespace util {

constexpr int RSA_STRENGTH = 2048;
constexpr int RSA_SIGNATURE_SIZE = RSA_STRENGTH / 8;

using SignatureArray = std::array<uint8_t, RSA_SIGNATURE_SIZE>;

/**
 * The interface for all cryptography functions needed by the meter client protocols.
 * This is a class rather than a set of functions because some cryptography libraries
 * need to maintain state between successive function invocations. (The simulation
 * implementation of this won't actually do any cryptography).
 */
class CryptoLibrary {
    public:
        /**
         * Encrypts the body of an OverlayMessage under the public key of the given meter.
         * @param message
         * @param target_meter_id The ID of the meter whose public key should be used to encrypt
         * @return An OverlayMessage with an encrypted body
         */
        virtual std::shared_ptr<messaging::OverlayMessage> rsa_encrypt(
                const std::shared_ptr<messaging::OverlayMessage>& message, const int target_meter_id) = 0;

        /**
         * Decrypts the body of an encrypted OverlayMessage, using the private
         * key of the current client.
         * @param message An OverlayMessage with an encrypted body
         * @return The same OverlayMessage, decrypted.
         */
        virtual std::shared_ptr<messaging::OverlayMessage> rsa_decrypt(
                const std::shared_ptr<messaging::OverlayMessage>& message) = 0;

        /**
         * Creates a blinded message representing a ValueTuple, by multiplying
         * its numeric representation by a random value that is invertible under
         * the RSA modulus of utility's public key. (Blinded messages are only ever
         * sent to the utility.)
         * @param value The value to blind
         * @return A byte sequence containing the blinded message.
         */
        virtual std::shared_ptr<messaging::StringBody> rsa_blind(
                const std::shared_ptr<messaging::ValueTuple>& value) = 0;

        /**
         * Signs a blinded message with the current client's private key.
         * This should not be used to sign any other kind of message, because
         * it uses a nonstandard "raw" signature.
         * @param encrypted_message A blinded message to sign, which will be
         * stored as a sequence of bytes and sent in an OverlayMessage (hence
         * the StringBody type).
         * @return A blind signature over the message, which is also a
         * sequence of bytes represented as a StringBody
         */
        virtual std::shared_ptr<messaging::StringBody> rsa_sign_blinded(
                const std::shared_ptr<messaging::StringBody>& blinded_message) = 0;

        /**
         * Unblinds a signature using the inverse of the blinding value this
         * client most recently used. This only works because blind-signature
         * requests are sent sequentially (and to only one destination,
         * the utility). The unblinded signature is placed in the SignatureArray
         * parameter.
         * @param blinded_signature The blinded signature to unblind
         * @param signature The unblinded signature
         */
        virtual void rsa_unblind_signature(const std::shared_ptr<std::string>& blinded_signature,
                SignatureArray& signature) = 0;

        /**
         * Encrypts a ValueTuple under under the public key of the given meter.
         * @param value The ValueTuple to encrypt
         * @param target_meter_id The ID of the meter whose public key should
         * be used to encrypt the ValueTuple
         * @return A string representing the bits of the ciphertext (as a
         * StringBody since it will probably be sent in a message)
         */
        virtual std::shared_ptr<messaging::StringBody> rsa_encrypt(
                const std::shared_ptr<messaging::ValueTuple>& value, const int target_meter_id) = 0;

//        virtual std::shared_ptr<messaging::ValueTuple> rsa_decrypt(
//                const std::shared_ptr<messaging::MessageBody>& value) = 0;

        /**
         * Signs a ValueContribution with the current client's private key, and
         * places the resulting signature in the SignatureArray
         * @param value The ValueContribution to sign
         * @param signature The signature over the ValueContribution
         */
        virtual void rsa_sign(const messaging::ValueContribution& value, SignatureArray& signature) = 0;

        /**
         * Verifies the signature on a ValueContribution against the public key
         * of the meter with the given ID.
         * @param value The ValueContribution that the signature covers
         * @param signature The signature on the ValueContribution
         * @param signer_meter_id The ID of the meter that supposedly produced
         * the signature
         * @return True if the signature is valid, false if it is not
         */
        virtual bool rsa_verify(const messaging::ValueContribution& value,
                const SignatureArray& signature, const int signer_meter_id) = 0;

        /**
         * Signs a SignedValue with the current client's private key, and places
         * the resulting signature in the SignatureArray
         * @param value The SignedValue to sign
         * @param signature The signature over the SignedValue
         */
        virtual void rsa_sign(const messaging::SignedValue& value, SignatureArray& signature) = 0;

        /**
         * Verifies the signature on a SignedValue against the public key of
         * the meter with the given ID.
         * @param value The SignedValue that the signature covers
         * @param signature The signature on the SignedValue
         * @param signer_meter_id The ID of the meter that supposedly produced
         * the signature
         * @return True if the signature is valid, false if it is not
         */
        virtual bool rsa_verify(const messaging::SignedValue& value, const SignatureArray& signature,
                const int signer_meter_id) = 0;

        virtual ~CryptoLibrary() = 0;
};

inline CryptoLibrary::~CryptoLibrary() { }

} /* namespace util */
} /* namespace pddm */


