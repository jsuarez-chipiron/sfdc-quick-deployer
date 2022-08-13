/*
Author:         R. cervino
Company:        Deloitte
Description:    Obtain signsture status
History:
<Date>          <Author>            <Description>
02/03/2021     R. Cervino          Initial version
 */
global class WS_signatureGetStatus {

    global static String getSignatureStatus(String operationGlobalId, String sign_id, String country, String codigoBic){
        try {
            CNT_ApiConfiguration__c configWs = CNT_ApiConfiguration__c.getValues('WS_SignatureGetStatus');
            String finalEndPoint = configWs.CNT_Endpoint__c.replace('{sign_id}', sign_id);            
            Map<String, String> extraHeader = new Map<String, String>();
            extraHeader.put('Global-Payment-Id', operationGlobalId);
            extraHeader.put('Access-Channel', 'web');
            String res = Global_IntegrationWrapper.launchHTTP('',finalEndPoint,configWs.CNT_HTTPMethod__c, extraHeader, 25000);   
            if(String.isNotBlank(res)){
                WS_signatureGetStatus.OUTPUT signatureStatus = (WS_signatureGetStatus.OUTPUT) JSON.deserialize(res, WS_signatureGetStatus.OUTPUT.class);
                if(String.isBlank(signatureStatus.signId) /*|| signatureStatus.signId != sign_id*/){
                    throw new CustomException('getSignatureStatus: wrong or empty signId');
                } else {
                    if(String.isNotBlank(signatureStatus.status)){
                        //Possible values: requested, denied, accepted and timeout
                        if(signatureStatus.status == 'accepted'){
                            CNT_B2B_SignatureUtilities.setSignature(sign_id);
                        }
                        return signatureStatus.status;
                    } else {
                        throw new CustomException('getSignatureStatus: wrong or empty status');
                    }
                }
            } else {
                throw new CustomException('getSignatureStatus: empty response');
            }
        } catch (Exception e) {
            throw new CustomException('getSignatureStatus: ' + e.getMessage() + ', in line: ' + e.getLineNumber());
        }
    }

    global class OUTPUT{
        global String signId;
        global String status;

        global OUTPUT(){}
    }
}
