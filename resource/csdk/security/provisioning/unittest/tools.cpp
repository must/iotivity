/* *****************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "octhread.h"
#include "srmutility.h"
#include "tools.h"
#include "oic_malloc.h"
#include "srmresourcestrings.h"
#include "logger.h"
#include "oic_string.h"
#include "ocprovisioningmanager.h"

#ifdef __cplusplus
}
#endif

#ifndef TAG
#define TAG "LIB_UNITTEST"
#endif

OicSecDoxm_t *createDoxm()
{
    OicSecDoxm_t *doxm = (OicSecDoxm_t *)OICMalloc(sizeof(OicSecDoxm_t));
    if (NULL == doxm)
    {
        return NULL;
    }
    doxm->oxmLen     = 1;
    doxm->oxm        = (OicSecOxm_t *)OICCalloc(doxm->oxmLen, sizeof(OicSecOxm_t));
    doxm->oxm[0]     = OIC_JUST_WORKS;
    doxm->oxmSel     = OIC_JUST_WORKS;
    doxm->sct        = SYMMETRIC_PAIR_WISE_KEY;
    doxm->owned      = true;
    ConvertStrToUuid("33333333-3333-3333-3333-111111111111", &doxm->deviceID);
    ConvertStrToUuid("33333333-3333-3333-3333-222222222222", &doxm->owner);
    return doxm;
}

void freeDoxm(OicSecDoxm_t *doxm)
{
    if (NULL == doxm)
    {
        return;
    }
    OICFree(doxm->oxm);
    OICFree(doxm);
}

OCDevAddr *setAddr(const char *ip, uint16_t port, OCTransportAdapter adapter)
{
    OCDevAddr *a = (OCDevAddr *)OICCalloc(1, sizeof(OCDevAddr));
    strcpy(a->addr, ip);
    a->port = port;
    a->adapter = adapter;
    return a;
}

OCProvisionDev_t *createProvisionDev()
{
    OCProvisionDev_t *pd  = (OCProvisionDev_t *)OICCalloc(1, sizeof(OCProvisionDev_t));
    strcpy(pd->endpoint.addr, "127.0.0.1");
    pd->endpoint.port = 9999;
    pd->endpoint.adapter = OC_ADAPTER_IP;
    pd->pstat = (OicSecPstat_t *)OICCalloc(1, sizeof(OicSecPstat_t));
    pd->doxm = createDoxm();
    pd->connType = (OCConnectivityType)(CT_ADAPTER_IP | CT_FLAG_SECURE | CT_IP_USE_V4);
    pd->securePort = 1023;
#ifdef WITH_TCP
    pd->tcpPort = 0;
    pd->tcpSecurePort = 0;
#endif
    OICStrcpy(pd->specVer, SPEC_MAX_VER_LEN, "res.2.0.0");
    pd->devStatus = DEV_STATUS_ON;
    pd->handle = NULL;
    pd->ownerAclUnauthorizedRequest = false;
    pd->next = NULL;
    return pd;
}

void freeProvisionDev(OCProvisionDev_t *pd)
{
    if (NULL == pd)
    {
        return;
    }
    OICFree(pd->pstat);
    freeDoxm(pd->doxm);
    OICFree(pd);
}

static void provisionResultCB(void *ctx, size_t nOfRes, OCProvisionResult_t *arr, bool hasError)
{
    OC_UNUSED(ctx);
    OC_UNUSED(nOfRes);
    OC_UNUSED(arr);
    OC_UNUSED(hasError);
    OIC_LOG_V(DEBUG, TAG, "%s: %d", __func__, hasError);
}

OTMContext_t *createOTMContext()
{
    OTMContext_t *otmCtx = (OTMContext_t *)OICCalloc(1, sizeof(OTMContext_t));
    otmCtx->selectedDeviceInfo = createProvisionDev();
    otmCtx->ctxResultCallback = provisionResultCB;
    return otmCtx;
}

void freeOTMContext(OTMContext_t *otmCtx)
{
    if (NULL == otmCtx)
    {
        return;
    }
    freeProvisionDev(otmCtx->selectedDeviceInfo);
    OICFree(otmCtx);
}

CAEndpoint_t *createEndpoint()
{
    CAEndpoint_t *e = (CAEndpoint_t *)OICCalloc(1, sizeof(CAEndpoint_t));
    e->adapter = CA_ADAPTER_IP;
    e->flags = (CATransportFlags_t)(CA_SECURE | CA_IPV4);
    e->port = 1024;
    strcpy(e->addr, "127.0.0.1");
    e->ifindex = 0;
//    e->remoteId[CA_MAX_IDENTITY_SIZE];
#if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
//    char                    routeData[MAX_ADDR_STR_SIZE_CA];
#endif
    return e;
}

void freeEndpoint(CAEndpoint_t *endpoint)
{
    OICFree(endpoint);
}

OicUuid_t *createUuid()
{
    OicUuid_t *uuid = (OicUuid_t *)OICCalloc(1, sizeof(OicUuid_t));
    ConvertStrToUuid("33333333-3333-3333-3333-222222222222", uuid);
    return uuid;
}

OicUuid_t *createUuidWith(const char *uuidStr)
{
    OicUuid_t *uuid = (OicUuid_t *)OICCalloc(1, sizeof(OicUuid_t));
    ConvertStrToUuid(uuidStr, uuid);
    return uuid;
}


void freeUuid(OicUuid_t *uuid)
{
    OICFree(uuid);
}

const char *getPemKey()
{
    static const char key [] ="-----BEGIN PRIVATE KEY-----\
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCmg+2hGPWQUDey\
NfXNJQnXVVM+tPl+Neu85BjYH1A6q1oJV0+qGhNiT6u3MX8iIn8EvezkGkqgoz2G\
sUAaeHcsjJYMQFmYsCNtadiE3rDy/g2qfCy8bMUKiIuagf1FnXv4+b+Z5pIDxnBd\
xDiNvQjeeA6bn1si1oQenM8vf/TnyCh9sXlh/+E1TANaVpr7r/pXOmDLgvBkSs5j\
65x1ZAlUpRBgEiFaB2yPWfxM76KuyPEXy9ZJ0/o67TAVPMiV5zID8nh2UO+w37eF\
jmqRR4UO1gRflJuiODJ6aupRhQiU8VUBXy6KKEA+1+p1ztpoDONyaMRen+nCxNKB\
bARlNlGFAgMBAAECggEBAIfeUDBJ8F0Px1JniygX177XhXQ+MPGVx8O6DreSXWFx\
3aaPkpZGbpiiT4fMIGNW6HOTEU9SUKBgclyMFrVCpGM6V1ipzTG0T0QAwMYx+wpa\
2AXLCqm4ytJ8dgTdmi6KgNS33GoUZAImkhtAd7DXlPwJ1EmRLsjvZqCf7MJiUxAn\
xkhd5DdD04fNGflt/QHEPqlMe8jGrTpKJn2ZX3lR3a7xTjTOCNyuEB2BSq27BKUg\
ki/2NXBBPnOonvrNx1RvgmoXp5zSApL9WAuL4iI4xeReqMJUZDTqooGO9v+sCD7C\
6I8wnBOQl5JyAwQFzdsE5QGofbZifF0iMVZkP0TVZeUCgYEAz+6y0+Ebf1KkrkvG\
fQIC6EMyNHuGRTCT3x2XFjPJUP3fX/kqmRSOax4QqgGRDbGPMr0yCzinFWrxV9Tp\
BOgaI96wMSKsfSDtlDmXLBDGSZaCID101n6tK+wpH/ZZqtQ0aaVlN+PrUwHyXNeC\
0/LcXx/a8izV6OKpUP69rPFpZssCgYEAzQIx8IAVNrEFViBvHvyQ/bwGzu4x50D8\
6BrFdf2UiigtegcIeuC/o2EWzh9Hr/lxTGn8NVLL0s2aqOme/vMBucWFwIO5Dgxp\
LGv8//+qQ3lDbBoOivjMVOr14qm0Y2Suu61uZgUYRkziKCRihqq3o0nsx6TXRmS1\
L14R4p5Izu8CgYATrjkY2taab30MzjlEPbt+tlV7KDHmPJnSZV+Wh2JHmy/YSqIg\
iRSNyPO0058iF6tBFy00aLFP0qZzNMC1aS6INJyimfCPi11RST4oupG6B1fbq1Vy\
s9y6Kokd8Fg+VgV79l6lMppkaevBo9hLLVJ0YnR7tYitmT6DXwke5F6gjwKBgQCO\
Wh7ZM+cHEgZ1lpHyf0gimCRWHfnS7UJv3N8OHiW9hUaGvEl8IYGQbgNK31eE7v1o\
JRP25uYXG6Nv9MDtoUmqeGh2lEC0a5PAP7/u9tWAgrx5XOKKR1aDBJ/2YwaaGbhn\
I3dMKe+AKTTE0PcFMZfQL63VtzWwcI+LM5PRuz0tSwKBgBXtbXKFqYmIsPrCi1t8\
spliEwNPJqCyUdE0ZM/pLXl4AVtL3yTdGkTysqlxxLTQZtgZBq316uFIQhM2aSI+\
FT658IakILe/pcAMZEtz7j2+C9B7hOxPiQYi5WgVx2EB8ishJWO59Rjy3npyHLou\
arVShNdQS03YzFPzRpRcb03C\
-----END PRIVATE KEY-----";
    return key;
}

const char *getPemCert()
{
    static const char cert[] =  "-----BEGIN CERTIFICATE-----\
MIID9TCCAt2gAwIBAgIJAK9okU/M3zDXMA0GCSqGSIb3DQEBCwUAMIGQMQswCQYD\
VQQGEwJVQTENMAsGA1UECAwES3lpdjENMAsGA1UEBwwES3lpdjEUMBIGA1UECgwL\
c2Ftc3VuZy5jb20xDzANBgNVBAsMBkFkU0VDTDEUMBIGA1UEAwwLc2Ftc3VuZy5j\
b20xJjAkBgkqhkiG9w0BCQEWF28uZG15dHJlbmtvQHNhbXN1bmcuY29tMB4XDTE5\
MDExODA3NTQxNFoXDTIwMDExODA3NTQxNFowgZAxCzAJBgNVBAYTAlVBMQ0wCwYD\
VQQIDARLeWl2MQ0wCwYDVQQHDARLeWl2MRQwEgYDVQQKDAtzYW1zdW5nLmNvbTEP\
MA0GA1UECwwGQWRTRUNMMRQwEgYDVQQDDAtzYW1zdW5nLmNvbTEmMCQGCSqGSIb3\
DQEJARYXby5kbXl0cmVua29Ac2Ftc3VuZy5jb20wggEiMA0GCSqGSIb3DQEBAQUA\
A4IBDwAwggEKAoIBAQCmg+2hGPWQUDeyNfXNJQnXVVM+tPl+Neu85BjYH1A6q1oJ\
V0+qGhNiT6u3MX8iIn8EvezkGkqgoz2GsUAaeHcsjJYMQFmYsCNtadiE3rDy/g2q\
fCy8bMUKiIuagf1FnXv4+b+Z5pIDxnBdxDiNvQjeeA6bn1si1oQenM8vf/TnyCh9\
sXlh/+E1TANaVpr7r/pXOmDLgvBkSs5j65x1ZAlUpRBgEiFaB2yPWfxM76KuyPEX\
y9ZJ0/o67TAVPMiV5zID8nh2UO+w37eFjmqRR4UO1gRflJuiODJ6aupRhQiU8VUB\
Xy6KKEA+1+p1ztpoDONyaMRen+nCxNKBbARlNlGFAgMBAAGjUDBOMB0GA1UdDgQW\
BBQkTeNZnWU5Dj608WEwAePuiEPBbjAfBgNVHSMEGDAWgBQkTeNZnWU5Dj608WEw\
AePuiEPBbjAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQAQfBlbA+gi\
Yu/awzVDY8fJKZy9eC+cckrVdt3lrfzJuj7ckIvMm5JZryTLjlIAnkMSgjvTmmPe\
cH5RDYvgOUY+SxPWXE+fuYRrLONPtEBbRPgjMHlgln9U/teU/BBYkjUm0VGnBXdr\
N3SKb16ixlrtQwNk7MoSkKTk4oQU4QeNKW2SZF7vOHeu2zK6W31sqXgzc3cvBySN\
6NLZvTYy3ohuFdK2krj1YExxFCBLvSc5fmOiv/7cZl5wsOnHqiYSkoQwm6rnVCGV\
X/MH0JzgkxxnY5qbBLTFqxdr+6yarTbDvWBpv2OrbarmuqyKBjs/7NHCa5zpnAWw\
sR1iytctB/HY\
-----END CERTIFICATE-----";
    return cert;
}


static uint8_t _certData[] =
{
    0x30, 0x82, 0x02, 0x39, 0x30, 0x82, 0x01, 0xdf, 0x02, 0x01, 0x01, 0x30, 0x0a, 0x06, 0x08, 0x2a,
    0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x7c, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x09, 0x53, 0x6f, 0x6d, 0x65, 0x73, 0x74, 0x61, 0x74, 0x65, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03,
    0x55, 0x04, 0x07, 0x0c, 0x08, 0x53, 0x6f, 0x6d, 0x65, 0x63, 0x69, 0x74, 0x79, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x02, 0x42, 0x42, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03,
    0x55, 0x04, 0x0b, 0x0c, 0x0d, 0x53, 0x65, 0x71, 0x75, 0x72, 0x69, 0x74, 0x79, 0x20, 0x50, 0x61,
    0x72, 0x74, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x02, 0x6f, 0x62, 0x31,
    0x14, 0x30, 0x12, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x05,
    0x6f, 0x62, 0x40, 0x62, 0x62, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x38, 0x31, 0x35, 0x31,
    0x33, 0x31, 0x31, 0x31, 0x37, 0x5a, 0x17, 0x0d, 0x31, 0x39, 0x30, 0x35, 0x31, 0x32, 0x31, 0x33,
    0x31, 0x31, 0x31, 0x37, 0x5a, 0x30, 0x81, 0xd4, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
    0x06, 0x13, 0x02, 0x55, 0x41, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x03,
    0x41, 0x73, 0x64, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x06, 0x47, 0x6f,
    0x74, 0x68, 0x61, 0x6d, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x02, 0x5a,
    0x5a, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x08, 0x42, 0x65, 0x61, 0x6d,
    0x54, 0x65, 0x61, 0x6d, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x09, 0x01, 0x16, 0x0d, 0x72, 0x61, 0x69, 0x6c, 0x40, 0x6d, 0x61, 0x69, 0x6c, 0x2e, 0x63,
    0x6f, 0x6d, 0x31, 0x32, 0x30, 0x30, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x29, 0x75, 0x75, 0x69,
    0x64, 0x3a, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 0x2d, 0x33, 0x32, 0x33, 0x32, 0x2d,
    0x33, 0x32, 0x33, 0x32, 0x2d, 0x33, 0x32, 0x33, 0x32, 0x2d, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32,
    0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 0x31, 0x34, 0x30, 0x32, 0x06, 0x03, 0x55, 0x1d, 0x11, 0x0c,
    0x2b, 0x75, 0x73, 0x65, 0x72, 0x69, 0x64, 0x3a, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37,
    0x2d, 0x36, 0x37, 0x36, 0x37, 0x2d, 0x36, 0x37, 0x36, 0x37, 0x2d, 0x36, 0x37, 0x36, 0x37, 0x2d,
    0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x30, 0x59, 0x30, 0x13,
    0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xf7, 0x13, 0x5c, 0x73, 0x72, 0xce, 0x10, 0xe5, 0x09,
    0x97, 0x9a, 0xf8, 0xf2, 0x70, 0xa6, 0x3d, 0x89, 0xf5, 0xc5, 0xe4, 0x44, 0xe2, 0x4a, 0xb6, 0x61,
    0xa8, 0x12, 0x8d, 0xb4, 0xdc, 0x2b, 0x47, 0x84, 0x60, 0x0c, 0x25, 0x66, 0xe9, 0xe0, 0xe5, 0xac,
    0x22, 0xbf, 0x15, 0xdc, 0x71, 0xb1, 0x88, 0x4f, 0x16, 0xbf, 0xc2, 0x77, 0x37, 0x76, 0x3f, 0xe0,
    0x67, 0xc6, 0x1d, 0x23, 0xfe, 0x7c, 0x8b, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x20, 0x47, 0xcc, 0x41, 0x8a, 0x27, 0xc7,
    0xd0, 0xaa, 0xb4, 0xab, 0x85, 0xbf, 0x09, 0x4d, 0x06, 0xd7, 0x7e, 0x0d, 0x39, 0xf9, 0x36, 0xa1,
    0x3d, 0x96, 0x23, 0xe2, 0x24, 0x64, 0x98, 0x63, 0x21, 0xba, 0x02, 0x21, 0x00, 0xe5, 0x8f, 0x7f,
    0xf1, 0xa6, 0x82, 0x03, 0x6a, 0x18, 0x7a, 0x54, 0xe7, 0x0e, 0x25, 0x77, 0xd8, 0x46, 0xfa, 0x96,
    0x8a, 0x7e, 0x14, 0xc4, 0xcb, 0x21, 0x32, 0x3e, 0x89, 0xd9, 0xba, 0x8c, 0x3f
};

uint8_t *certData()
{
    uint8_t *buf = (uint8_t *)OICCalloc(1, sizeof(_certData));
    memcpy(buf, _certData, sizeof(_certData));
    return buf;
}

static uint8_t _keyData[] =
{
    0x67, 0xc6, 0x1d, 0x23, 0xfe, 0x7c, 0x8b, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x20, 0x47, 0xcc, 0x41, 0x8a, 0x27, 0xc7,
    0xd0, 0xaa, 0xb4, 0xab, 0x85, 0xbf, 0x09, 0x4d, 0x06, 0xd7, 0x7e, 0x0d, 0x39, 0xf9, 0x36, 0xa1,
    0x3d, 0x96, 0x23, 0xe2, 0x24, 0x64, 0x98, 0x63, 0x21, 0xba, 0x02, 0x21
};

uint8_t *keyData()
{
    uint8_t *buf = (uint8_t *)OICCalloc(1, sizeof(_keyData));
    memcpy(buf, _keyData, sizeof(_keyData));
    return buf;
}

size_t keyDataLen()
{
    return sizeof(_keyData);
}

size_t certDataLen()
{
    return sizeof(_certData);
}

OicSecCred_t *createCred()
{
    OicSecCred_t *cred = (OicSecCred_t *)OICCalloc(1, sizeof(OicSecCred_t ));
    cred->credId = 33;
    ConvertStrToUuid("33333333-3333-3333-3333-222222222222", &cred->subject);
    // If roleId.id is all zeroes, this property is not set.
    memset(cred->roleId.id, 0, ROLEID_LENGTH);         // 2:R:M:N:oic.sec.roletype
    memset(cred->roleId.authority, 0, ROLEAUTHORITY_LENGTH);         // 2:R:M:N:oic.sec.roletype
    cred->credType = SYMMETRIC_PAIR_WISE_KEY;       // 3:R:S:Y:oic.sec.credtype
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    cred->publicData.len = sizeof(_certData);
    cred->publicData.data = (uint8_t *)OICMalloc(cred->publicData.len);
    cred->publicData.encoding = OIC_ENCODING_DER;
    memcpy(cred->publicData.data, _certData, cred->publicData.len);
    cred->credUsage = NULL;            // 4:R:S:N:String
    cred->optionalData.data = NULL;   // CA's cerificate chain
    cred->optionalData.len = 0;   // CA's cerificate chain
    cred->optionalData.encoding = OIC_ENCODING_UNKNOW;
#endif /* __WITH_DTLS__  or __WITH_TLS__*/
    cred->privateData.len = sizeof(_keyData);
    cred->privateData.data = (uint8_t *)OICMalloc(cred->privateData.len);
    cred->privateData.encoding = OIC_ENCODING_DER;
    memcpy(cred->privateData.data, _keyData, cred->privateData.len);
    cred->period = NULL;
#ifdef MULTIPLE_OWNER
    ConvertStrToUuid("33333333-3333-3333-3333-222222222222", cred->eownerID);
#endif //MULTIPLE_OWNER
    cred->next = NULL;

    return cred;
}

#define DEFAULTPROFILE  "1.3.6.1.4.1.51414.0.0.1.0"

OicSecSp_t *getSpDefault()
{
    OicSecSp_t *sp = (OicSecSp_t *)OICCalloc(1, sizeof(OicSecSp_t));
    sp->supportedLen = 1;
    sp->supportedProfiles = (char **)OICCalloc(1, sizeof(char *));
    sp->supportedProfiles[0] = OICStrdup(DEFAULTPROFILE);
    sp->currentProfile = OICStrdup(DEFAULTPROFILE);
    return sp;
};
/*
char * gSupportedProfilesAll[] = { "1.3.6.1.4.1.51414.0.0.2.0", "1.3.6.1.4.1.51414.0.0.3.0", "1.3.6.1.4.1.51414.0.0.4.0", "1.3.6.1.4.1.51414.0.0.1.0",  };
OicSecSp_t gSpAll =
{
    4,                      // supportedLen
    gSupportedProfilesAll,  // supportedProfiles[0]
    "1.3.6.1.4.1.51414.0.0.2.0",     // activeProfile
};
char * gSupportedProfilesInvalid[] = { "1.3.6.1.4.1.51414.0.0.2.0", "1.3.6.1.4.1.51414.0.0.3.0", "1.3.6.1.4.1.51414.0.0.4.0", "1.3.6.1.4.1.51414.0.0.1.0",  };
OicSecSp_t gSpInvalid =
{
    4,                         // supportedLen
    gSupportedProfilesInvalid, // supportedProfiles
    "oic.sec.sp.invalid",      // activeProfile
};
*/
OicSecPstat_t *createPstat()
{
    OicSecPstat_t *pstat = (OicSecPstat_t *)OICCalloc(1, sizeof(OicSecPstat_t));
    pstat->dos.state = DOS_RFNOP;
    pstat->dos.pending = false;
    pstat->isOp = false;
    pstat->cm = NORMAL;
    pstat->tm = NORMAL;
    pstat->om = NORMAL;
    pstat->smLen = 1;
    pstat->sm = (OicSecDpom_t *)OICCalloc(pstat->smLen, sizeof(OicSecDpom_t ));
    pstat->sm[0] = NORMAL;
    pstat->commitHash = 0;
    ConvertStrToUuid("33333333-3333-3333-3333-222222222222", &pstat->rownerID);
    return pstat;
}

bool pInternalWorking;
oc_thread pInternalThread;

void *IOT_process(void *p)
{
    OIC_LOG_V(DEBUG, TAG, "%s: IN",__func__);
    OC_UNUSED(p);
    struct timespec timeout = {0, 100000000L};
    while (pInternalWorking)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(FATAL, TAG, "OCStack process error");
            return NULL;
        }
        nanosleep(&timeout, NULL);
    }
    OIC_LOG_V(DEBUG, TAG, "%s: OUT",__func__);
    return NULL;
}

FILE *fopenDb(const char *path, const char *mode)
{
    OC_UNUSED(path);
    return fopen("oic_svr_db_client.dat", mode);
}

void IOT_Init(const char *pDbName)
{
    OIC_LOG_V(DEBUG, TAG, "%s: IN",__func__);
#ifdef HAVE_UNISTD_H
    if (-1 != access(pDbName, F_OK))
    {
        unlink(pDbName);
    }
#endif // HAVE_UNISTD_H
#   if (__STDC_VERSION__ >= 199901L)
    static OCPersistentStorage gPst = {.open = fopenDb, .read = fread, .write = fwrite, .close = fclose, .unlink = unlink};
#   else
    static OCPersistentStorage gPst = {fopenDb, fread, fwrite, fclose, unlink};
#   endif

    EXPECT_EQ(OC_STACK_OK, OCRegisterPersistentStorageHandler(&gPst));
    EXPECT_EQ(OC_STACK_OK, OCInit(NULL, 0, OC_CLIENT_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCInitPM(pDbName));

    pInternalWorking = true;
    oc_thread_new(&pInternalThread, IOT_process, NULL);
    OIC_LOG_V(DEBUG, TAG, "%s: OUT",__func__);
}

void IOT_DeInit(const char *pDbName)
{
    OIC_LOG_V(DEBUG, TAG, "%s: IN",__func__);
    pInternalWorking = false;

    EXPECT_EQ(OC_STACK_OK, OCClosePM());
    EXPECT_EQ(OC_STACK_OK, OCStop());

    unlink(pDbName);

    oc_thread_wait(pInternalThread);
    oc_thread_free(pInternalThread);
    OIC_LOG_V(DEBUG, TAG, "%s: OUT",__func__);
}

