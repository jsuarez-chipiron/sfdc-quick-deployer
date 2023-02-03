# sfdc-quick-deployer

* Refactor to split in diffetent files ✓
* Create the DB withs the id's of the resources ✓
* Implement *update class* ✓
* Implement a full retrieve for all classes in an org (select Id, name from apexclass) ✓
* Implement a verbose mode with the output of the services
* Implement a mode to see the logged details
* Create a server to clean async the tooling api resources


** Usage

1. Update Login Details:
./sfdc-quick-deployer -l -u username@force.com -p password -r https://login.salesforce.com

2. Get all metadata from org into the dictionary
./sfdc-quick-deployer -g

3. Download a resource
./sfdc-quick-deployer -d -f ../resources/LT1_Ctrl.cls

4. Upload a resource
./sfdc-quick-deployer -f ../resources/LT1_Ctrl.cls
