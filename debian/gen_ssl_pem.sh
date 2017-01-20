#!/bin/sh

# These defaults were extracted from Synergy source file:
# src/gui/src/SslCertificate.cpp
# 
# A private key and certificate is generated in the profile
# directory (~/.synergy/SSL/Synergy.pem). Then a fingerprint
# is generated that can be used to verify the client is
# connecting to the correct server
# (~/.synergy/SSL/Fingerprints/Local.txt)

if hash syntool 2>/dev/null; then
    if hash openssl 2>/dev/null; then
        mkdir -p "$(syntool --get-profile-dir)/SSL" && openssl req -x509 -nodes -days 365 -subj '/CN=Synergy' -newkey rsa:1024 -keyout "$(syntool --get-profile-dir)/SSL/Synergy.pem" -out "$(syntool --get-profile-dir)/SSL/Synergy.pem" && mkdir -p "$(syntool --get-profile-dir)/SSL/Fingerprints/" &&  openssl x509 -fingerprint -sha1 -noout -in "$(syntool --get-profile-dir)/SSL/Synergy.pem" | cut -d= -f2 > "$(syntool --get-profile-dir)/SSL/Fingerprints/Local.txt"
    else
        echo "openssl not found in path"
    fi
    echo "Server Fingerprint:"
    cat "$(syntool --get-profile-dir)/SSL/Fingerprints/Local.txt"
else
    echo "syntool not found in path"
fi

