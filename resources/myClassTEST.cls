public with sharing class LT1_Ctrl {

    @AuraEnabled
    public static Account getAccountById(String pId) {
        // javier javier
        return LT_Account_Service.getAccountById(pId);
    }
    
}
