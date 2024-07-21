#!/bin/bash

cp -v build/libquickdeployer.a /usr/local/lib

rm -vfr /usr/local/include/quickdeployer

mkdir /usr/local/include/quickdeployer

cp -vr src/orquestrator/ /usr/local/include/quickdeployer
cp -vr src/req_res_utils/ /usr/local/include/quickdeployer
cp -vr src/resource_repo/ /usr/local/include/quickdeployer
cp -vr src/serdes/ /usr/local/include/quickdeployer
cp -vr src/sfdc_client/ /usr/local/include/quickdeployer

for i in $(find /usr/local/include/quickdeployer -name "*.cpp")
do
    rm -v $i
done
