#!/bin/sh
PDFDOCUMENT=SampleSignedPDFDocument.pdf
ROOTCERT=`ls $PDFDOCUMENT.certificate.Ado*.pem`
CACERT=`ls $PDFDOCUMENT.certificate.Geo*.pem`
SIGNERCERT=`ls $PDFDOCUMENT.certificate.CA*.pem`
openssl smime -verify -in $PDFDOCUMENT.pkcs7 -inform der -content $PDFDOCUMENT.tbs -CAfile $ROOTCERT -signer $SIGNERCERT -out $PDFDOCUMENT.smime.p7s
