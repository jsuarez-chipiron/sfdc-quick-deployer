public with sharing class LT3_Ctrl {

    @AuraEnabled
    public static Account getAccountById(String pId) {
        // javier javier javier
        return LT_Account_Service.getAccountById(pId);
    }
    
}
