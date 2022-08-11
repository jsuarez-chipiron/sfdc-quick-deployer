#!/bin/bash

cd build && make && ./sfdc-quick-deployer -d -f ../resources/MySuperCoolTest.cls
