/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
package org.iotivity.cloud.accountserver.util;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * 
 * This class provides a set of APIs to parse JSON object and convert data
 * object to JSON string.
 * 
 */
public class JSONUtil {

    private static ObjectMapper mapper = new ObjectMapper();

    /**
     * API for parsing json string and getting value corresponding with key.
     * 
     * @param jsonString
     *            json string
     * @return String - value corresponding with key
     */
    public String parseJSON(String jsonString, String key) {

        if (jsonString == null || jsonString.equals(""))
            return null;

        String value = null;

        try {
            @SuppressWarnings("unchecked")
            Map<String, String> jsonMap = mapper.readValue(jsonString,
                    Map.class);
            value = jsonMap.get(key);
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }

        return value;
    }

    /**
     * API for converting data of HashMap-type to json string.
     * 
     * @param data
     *            data of HashMap-type
     * @return String - converted json string
     */
    public String writeJSON(HashMap<Object, Object> data) {
        if (data == null)
            return null;

        String json = null;
        try {
            json = mapper.writeValueAsString(data);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

        if (json == null)
            json = "{}";

        return json;
    }
}
