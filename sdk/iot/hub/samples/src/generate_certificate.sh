#!/bin/bash
# Copyright (c) Microsoft Corporation. All rights reserved.
# SPDX-License-Identifier: MIT

openssl ecparam -out device_ec_key.pem -name prime256v1 -genkey
openssl req -new -days 365 -nodes -x509 -key device_ec_key.pem -out device_ec_cert.pem -config x509_config.cfg -subj "/CN=paho-sample-device1"
openssl x509 -noout -text -in device_ec_cert.pem


rm device_cert_store.pem
cat device_ec_cert.pem device_ec_key.pem > device_cert_store.pem

echo -e "\nIt is NOT recommended to use OpenSSL on Windows or OSX. Recommended TLS stacks are:"
echo "Microsoft Windows SChannel: https://docs.microsoft.com/en-us/windows/win32/com/schannel"
echo "OR"
echo "Apple Secure Transport : https://developer.apple.com/documentation/security/secure_transport"
echo -e "\nIf using OpenSSL, it is recommended to use the OpenSSL Trusted CA store configured on your system."
echo "If required (for example on Windows), download the Baltimore PEM CA from https://www.digicert.com/digicert-root-certificates.htm to the current folder."
echo    export AZ_IOT_DEVICE_X509_TRUST_PEM_FILE=$(pwd)/BaltimoreCyberTrustRoot.crt.pem

echo -e "\nSample certificate generated"
echo "Use the device_cert_store.pem file within the sample:"
echo    export AZ_IOT_DEVICE_X509_CERT_PEM_FILE=$(pwd)/device_cert_store.pem

echo -e "\nUse the following fingerprint when creating your device in IoT Hub (must remove colons):"
openssl x509 -noout -fingerprint -in device_ec_cert.pem
