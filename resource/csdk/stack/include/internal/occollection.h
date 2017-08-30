//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef OC_COLLECTION_H
#define OC_COLLECTION_H

#include "ocstack.h"
#include "ocresourcehandler.h"

uint8_t GetNumOfResourcesInCollection(const OCResource *resource);

OCStackResult DefaultCollectionEntityHandler (OCEntityHandlerFlag flag,
                                              OCEntityHandlerRequest *entityHandlerRequest);

/**
 * This function creates the payloadValue for links parameter of collection resource.
 * @param[in] resourceUri Resource uri (this should be a collection resource)
 * @param[out] linksRepPayloadValue The payloadValue for links parameter of collection
 * @param[in] devAddr Structure pointing to the address. (from OCEntityHandlerRequest)
 * @param[in] isOCFVer true if AcceptedVersion is OCF1.0 or higher
 *            otherwise false in case OIC1.1 (from OCEntityHandlerRequest)
 *
 * @note: The destroy of OCRepPayloadValue is not supported. Instead, use
 *        OCRepPayloadDestroy(...) to destroy RepPayload of the collection Resource
 *
 * @return ::true if successful otherwise false.
 */
bool BuildCollectionLinksPayloadValue(const char* resourceUri,
                    OCRepPayloadValue** linksRepPayloadValue, bool isOCFContentFormat, OCDevAddr* devAddr);

#endif //OC_COLLECTION_H
