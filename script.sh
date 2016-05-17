#!/bin/sh
PDFDOCUMENT=SampleSignedPDFDocument.pdf
offset=`cat $PDFDOCUMENT |./byteR.start|head -1`
dd if=$PDFDOCUMENT skip=$((offset+10)) bs=1 count=1000 of=$PDFDOCUMENT.tmp
offset=`cat $PDFDOCUMENT.tmp |./byteR.end |head -1`
dd if=$PDFDOCUMENT.tmp skip=0 bs=1 count=$offset of=$PDFDOCUMENT.range
rm $PDFDOCUMENT.tmp


partype=0
for a in `cat $PDFDOCUMENT.range`;do
  if [ $partype -eq 0 ];then
    start=$a
    partype=1
  else
    len=$a
    echo $len
    echo $start
    partype=0
    dd if=$PDFDOCUMENT count=$len skip=$start bs=1 >> $PDFDOCUMENT.tbs
  fi
  done
rm $PDFDOCUMENT.range


contentBegin=`cat $PDFDOCUMENT |./content.start|head -1`
dd if=$PDFDOCUMENT skip=$((contentBegin+9)) bs=1 count=50000 of=$PDFDOCUMENT.tmp
contentEnd=`cat $PDFDOCUMENT.tmp |./content.end |head -1`
dd if=$PDFDOCUMENT.tmp skip=0 bs=1 count=$contentEnd of=$PDFDOCUMENT.signer
xxd -r -p $PDFDOCUMENT.signer > $PDFDOCUMENT.pkcs7
openssl pkcs7 -in $PDFDOCUMENT.pkcs7 -inform der -print_certs > $PDFDOCUMENT.certif
rm $PDFDOCUMENT.tmp $PDFDOCUMENT.signer


partype=0
noflines=`cat $PDFDOCUMENT.certif|wc -l`
for linenr in `grep -n subject $PDFDOCUMENT.certif|cut -d':' -f1` $((noflines+1));do
  if [ $partype -eq 0 ];then
    partype=1
    start=$linenr
  else
    end=$((linenr-1))
    head -$end $PDFDOCUMENT.certif|tail -$((linenr-$start+1)) > $PDFDOCUMENT.certificate.tmp
    subject=`grep -n subject $PDFDOCUMENT.certificate.tmp|head -1|sed s/'\/'/_/g|cut -d')' -f-1|sed s/\(//g|sed s/\)//g|sed s/' '/_/g|cut -d':' -f2|cut -d'=' -f4`
    mv $PDFDOCUMENT.certificate.tmp $PDFDOCUMENT.certificate.$subject.pem
    start=$((linenr+1))
  fi
  done
