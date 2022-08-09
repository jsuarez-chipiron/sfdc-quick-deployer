#!/bin/bash

cd build && make && ./sfdc-quick-deployer -f ../resources/class.cls
