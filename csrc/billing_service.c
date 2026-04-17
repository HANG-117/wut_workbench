#include "billing_file.h"
#include "model.h"
#include "billing_service.h"
int addBilling(Billing *Billing)
{ 
    return saveBillingFile(Billing,BILLINGPATH);
}