/*
	Copyright 2009-2012, Sumeet Chhetri 
  
    Licensed under the Apache License, Version 2.0 (const the& "License"); 
    you may not use this file except in compliance with the License. 
    You may obtain a copy of the License at 
  
        http://www.apache.org/licenses/LICENSE-2.0 
  
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, 
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
    See the License for the specific language governing permissions and 
    limitations under the License.  
*/
/*
 * HttpSession.h
 *
 *  Created on: Aug 11, 2009
 *      Author: sumeet
 */

#include "map"
#include <iostream>


#ifndef HTTPSESSION_H_
#define HTTPSESSION_H_
#include "HTTPResponseStatus.h"


typedef std::map<std::string, std::string, cicomp> Map;

class HttpSession {
	std::string sessionId;
	Map sessionAttributes;
	bool dirty;
public:
	HttpSession();
	virtual ~HttpSession();
    std::string getSessionId() const;
    void setSessionId(const std::string&);
    Map getSessionAttributes();
    void setSessionAttributes(const Map&);
    std::string getAttribute(const std::string&);
    void setAttribute(const std::string&, const std::string&);
    bool isDirty() const{return dirty;}
};

#endif /* HTTPSESSION_H_ */
