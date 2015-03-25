#include "defaults.h"
#include "ContainerResultData.h"
#include "ContainerData.h"
#include "AllAgentsResultData.h"
#include "AgentResultData.h"
#include "AgentData.h"
#include "CreateKeyResultData.h"
#include "ObjectConverter.h"
#include "Log.h"
#include "PushResultData.h"
#include "NotifyResultData.h"
#include "ResultData.h"
#include "CustomerData.h"
#include "CreateCustomerResultData.h"

using namespace ServerEye;

ObjectConverter::ObjectConverter()
{
}

ObjectConverter::~ObjectConverter()
{
}

// CONVERT JSON -> List<AGENT>
AllAgentsResultPtr ObjectConverter::GetAllAgentsResult(const wstring& json)
{
	AllAgentsResultDataPtr allAgentsDataPtr(new AllAgentsResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			// PARSE RESULT STUFF
			if(resultObj[i].name_ == L"success")
			{
				allAgentsDataPtr->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				if(allAgentsDataPtr->getSuccess())
				{
					// in success case message is an empty array -> we do nothing
				}
				else
				{
					// parse the error string
					allAgentsDataPtr->setMessage(resultObj[i].value_.get_str());
				}
			}
			else if(resultObj[i].name_ == L"data" && resultObj[i].value_.type() == json_spirit::array_type)
			{
				// PARSE ARRAY OF AGENTS
				json_spirit::wArray dataArray = resultObj[i].value_.get_array();
				AgentList list;

				for (size_t iData = 0; iData < dataArray.size(); iData++)
				{
					AgentDataPtr aData(new AgentData());

					// PARSE SINGLE AGENT
					json_spirit::wObject agent = dataArray[iData].get_obj();
					for (size_t iAgent = 0; iAgent < agent.size(); iAgent++)
					{
						if (agent[iAgent].value_.is_null()) continue; // handle 0 objects as empty objects

						else if (agent[iAgent].name_ == L"settings")
						{
							// Iterate over array of setting objects
							json_spirit::wArray settingsArray = (agent[iAgent].value_.get_array());
							for (size_t iSetting = 0; iSetting < settingsArray.size(); iSetting++)
							{
								// Get a single setting object
								json_spirit::wObject curSetting = settingsArray[iSetting].get_obj();
								AgentSettingPtr curAgentSettingPtr(new AgentData::AgentSettingData());

								// Parse the settings object 
								for (size_t itCurSetting = 0; itCurSetting < curSetting.size(); itCurSetting++)
								{
									if (curSetting[itCurSetting].name_ == L"key")
									{
										((AgentData::AgentSettingData*)curAgentSettingPtr.get())->setKey(curSetting[itCurSetting].value_.get_str());
									}
									else if (curSetting[itCurSetting].name_ == L"value")
									{
										curAgentSettingPtr->setValue(curSetting[itCurSetting].value_.get_str());
									}
									else if (curSetting[itCurSetting].name_ == L"definition")
									{
										curAgentSettingPtr->setDefinition(curSetting[itCurSetting].value_.get_str());
									}
									else if (curSetting[itCurSetting].name_ == L"orderInt")
									{
										curAgentSettingPtr->setOrderId(curSetting[itCurSetting].value_.get_int());
									}
								}
								// Add it to the agent
								aData->setSetting(curAgentSettingPtr);
							}
						}

						// PARSE AGENT STUFF
						else if(agent[iAgent].name_ == L"id")
						{
							aData->setId(agent[iAgent].value_.get_str());
						}
						else if(agent[iAgent].name_ == L"parentId")
						{
							aData->setParentId(agent[iAgent].value_.get_str());
						}
						else if(agent[iAgent].name_ == L"name")
						{
							aData->setName(agent[iAgent].value_.get_str());
						}
						else if(agent[iAgent].name_ == L"type")
						{
							aData->setType(agent[iAgent].value_.get_str());
						}
					}

					// Add parsed agent to list
					list.push_back(aData);
				}
				// Add the filled list to the result
				allAgentsDataPtr->setData(list);
			}
		}
	}

	return allAgentsDataPtr;
}

// CONVERT JSON -> AgentResultPtr
AgentResultPtr ObjectConverter::GetAgentResult(const wstring& json)
{
	AgentResultDataPtr aResultData(new AgentResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			if(resultObj[i].name_ == L"success")
			{
				aResultData->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				aResultData->setMessage(resultObj[i].value_.get_str());
			}
			else if(resultObj[i].name_ == L"data")
			{
				json_spirit::wObject dataObj = resultObj[i].value_.get_obj();
				AgentDataPtr aData(new AgentData());
				aResultData->setData(AgentPtr(aData));

				for (size_t iData = 0; iData < dataObj.size(); iData++)
				{
					if (dataObj[iData].value_.is_null()) continue; // handle 0 objects as empty objects

					else if(dataObj[iData].name_ == L"settings")
					{
						// Iterate over array of setting objects
						json_spirit::wArray settingsArray = (dataObj[iData].value_.get_array());
						for (size_t iSetting = 0; iSetting < settingsArray.size(); iSetting++)
						{
							// Get a single setting object
							json_spirit::wObject curSetting = settingsArray[iSetting].get_obj();
							AgentSettingPtr curAgentSettingPtr(new AgentData::AgentSettingData());

							// Parse the settings object 
							for(size_t itCurSetting = 0; itCurSetting < curSetting.size(); itCurSetting++)
							{
								if(curSetting[itCurSetting].name_ == L"key")
								{
									((AgentData::AgentSettingData*)curAgentSettingPtr.get())->setKey(curSetting[itCurSetting].value_.get_str());
								}
								else if(curSetting[itCurSetting].name_ == L"value")
								{
									curAgentSettingPtr->setValue(curSetting[itCurSetting].value_.get_str());
								}
								else if(curSetting[itCurSetting].name_ == L"definition")
								{
									curAgentSettingPtr->setDefinition(curSetting[itCurSetting].value_.get_str());
								}
								else if(curSetting[itCurSetting].name_ == L"orderInt")
								{
									curAgentSettingPtr->setOrderId(curSetting[itCurSetting].value_.get_int());
								}
							}
							// Add it to the agent
							aData->setSetting(curAgentSettingPtr);
						}
					}

					else if(dataObj[iData].name_ == L"id")
					{
						aData->setId(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"parentId")
					{
						aData->setParentId(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"name")
					{
						aData->setName(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"type")
					{
						aData->setType(dataObj[iData].value_.get_str());
					}
				}
			}
		}
	}

	return aResultData;
}


// CONVERT JSON -> ContainerResultPtr
ContainerResultPtr ObjectConverter::GetContainerResult(const wstring& json)
{
	ContainerResultDataPtr cResultData(new ContainerResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			if(resultObj[i].name_ == L"success")
			{
				cResultData->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				if(cResultData->getSuccess())
				{
					// in success case message is an empty array -> we do nothing
				}
				else
				{
					// parse the error string
					cResultData->setMessage(resultObj[i].value_.get_str());
				}
			}
			else if(resultObj[i].name_ == L"data")
			{
				json_spirit::wObject dataObj = resultObj[i].value_.get_obj();
				ContainerDataPtr cData(new ContainerData());
				cResultData->setData(ContainerPtr(cData));

				for (size_t iData = 0; iData < dataObj.size(); iData++)
				{
					if (dataObj[iData].value_.is_null()) continue; // handle 0 objects as empty objects

					// DATA
					if(dataObj[iData].name_ == L"id")
					{
						cData->setId(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"type")
					{
						cData->setContainerType((Container::ContainerType)dataObj[iData].value_.get_int());
					}
					else if(dataObj[iData].name_ == L"name")
					{
						cData->setName(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"customerId")
					{
						cData->setCustomerId(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"realName")
					{
						cData->setRealName(dataObj[iData].value_.get_str());
					}
					else if(dataObj[iData].name_ == L"maxHeartbeatTimeout")
					{
						cData->setMaxHeartbeatTimeout(dataObj[iData].value_.get_int());
					}
					else if(dataObj[iData].name_ == L"parentId")
					{
						cData->setParentId(dataObj[iData].value_.get_str());
					}

					// DATA -> SYSTEM
					else if(dataObj[iData].name_ == L"system")
					{
						json_spirit::wObject sysObj = dataObj[iData].value_.get_obj();
						for (size_t iSys = 0; iSys < sysObj.size(); iSys++)
						{
							if (sysObj[iSys].value_.is_null()) continue; // handle 0 objects as empty objects

							if(sysObj[iSys].name_ == L"ip")
							{
								cData->getSystem().setIp(sysObj[iSys].value_.get_str());
							}
							else if(sysObj[iSys].name_ == L"isServer")
							{
								cData->getSystem().setServer(sysObj[iSys].value_.get_bool());
							}
							else if(sysObj[iSys].name_ == L"isVm")
							{
								cData->getSystem().setVM(sysObj[iSys].value_.get_bool());
							}
							else if(sysObj[iSys].name_ == L"numberOfProcessors")
							{
								cData->getSystem().setNumberOfProcessors(sysObj[iSys].value_.get_int());
							}
							else if(sysObj[iSys].name_ == L"totalRam")
							{
								cData->getSystem().setTotalRam(sysObj[iSys].value_.get_int64());
							}
							else if(sysObj[iSys].name_ == L"architecture")
							{
								cData->getSystem().setArchitecture(sysObj[iSys].value_.get_int());
							}
						}
					}

					// DATA -> ADDITIONAL
					if(dataObj[iData].name_ == L"additional")
					{
						json_spirit::wObject addiObj = dataObj[i].value_.get_obj();
						for (size_t iAdd = 0; iAdd < addiObj.size(); iAdd++)
						{
							if (addiObj[iAdd].value_.is_null()) continue; // handle 0 objects as empty objects

							if(addiObj[iAdd].name_ == L"name")
							{
								cData->getAdditional().setName(addiObj[iAdd].value_.get_str());
							}
							else if(addiObj[iAdd].name_ == L"version")
							{
								cData->getAdditional().setVersion(addiObj[iAdd].value_.get_str());
							}
							else if(addiObj[iAdd].name_ == L"servicepack")
							{
								cData->getAdditional().setServicePack(addiObj[iAdd].value_.get_str());
							}
							else if(addiObj[iAdd].name_ == L"lastStart")
							{
								cData->getAdditional().setLastStart(addiObj[iAdd].value_.get_int64());
							}
						}
					}
				}
			}
		}
	}

	return cResultData;
}

// CONVERT JSON -> NotifyResult
NotifyResultPtr ObjectConverter::GetNotifyAgentResult(const wstring& json)
{
	NotifyResultDataPtr result(new NotifyResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			if(resultObj[i].name_ == L"success")
			{
				result->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				if(result->getSuccess())
				{
					// in success case message is an empty array -> we do nothing
				}
				else
				{
					// parse the error string
					result->setMessage(resultObj[i].value_.get_str());
				}
			}
			else if(resultObj[i].name_ == L"data")
			{
				// Currently data is not used
				json_spirit::wObject dataObj = resultObj[i].value_.get_obj();

				for (size_t idata = 0; idata < dataObj.size(); idata++) 
				{
				}
			}
		}
	}

	return result;
}

// CONVERT JSON -> CreateKeyResult
CreateKeyResultPtr ObjectConverter::GetCreateKeyResult(const wstring& json)
{
	CreateKeyResultDataPtr cResultData(new CreateKeyResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			if(resultObj[i].name_ == L"success")
			{
				cResultData->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				if(cResultData->getSuccess())
				{
					// in success case message is an empty array -> we do nothing
				}
				else
				{
					// parse the error string
					cResultData->setMessage(resultObj[i].value_.get_str());
				}
			}
			else if(resultObj[i].name_ == L"data")
			{
				json_spirit::wObject dataObj = resultObj[i].value_.get_obj();

				for (size_t idata = 0; idata < dataObj.size(); idata++) 
				{
					if(dataObj[idata].name_ == L"user_id")
					{
						cResultData->setUserId(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"customer_id")
					{
						cResultData->setCustomerId(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"apiKey")
					{
						cResultData->setAPIKey(dataObj[idata].value_.get_str());
					}
				}
			}
		}
	}

	return cResultData;
}

// CONVERT JSON -> PushResult
PushResultPtr ObjectConverter::GetPushResult(const wstring& json)
{
	PushResultDataPtr cPushResPtr(new PushResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			if(resultObj[i].name_ == L"success")
			{
				cPushResPtr->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				if(cPushResPtr->getSuccess())
				{
					cPushResPtr->setMessage((resultObj[i].value_.get_str()));
				}
				else
				{
					// parse the error string
					cPushResPtr->setMessage(resultObj[i].value_.get_str());
				}
			}
			else if(resultObj[i].name_ == L"data")
			{
				json_spirit::wObject dataObj = resultObj[i].value_.get_obj();

				for (size_t idata = 0; idata < dataObj.size(); idata++) 
				{
					if(dataObj[idata].name_ == L"rootContainerId")
					{
						cPushResPtr->setRootContainerId(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"containerId")
					{
						cPushResPtr->setContainerId(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"senderId")
					{
						cPushResPtr->setSenderId(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"messageId")
					{
						cPushResPtr->setMessageId(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"control")
					{
						cPushResPtr->setControl(dataObj[idata].value_.get_str());
					}
					else if(dataObj[idata].name_ == L"data")
					{
						cPushResPtr->setData(dataObj[idata].value_.get_str());
					}
				}
			}
		}
		// everything ok
		return cPushResPtr;
	}
	// No valid json data received
	return nullptr;
}

// CONVERT JSON -> Result
ResultPtr ObjectConverter::GetResult(const wstring& json)
{
	ResultDataPtr result(new ResultData());

	json_spirit::wValue value;
	if (json_spirit::read(json, value))
	{
		json_spirit::wObject resultObj = value.get_obj();

		for (size_t i = 0; i < resultObj.size(); i++)
		{
			if(resultObj[i].name_ == L"success")
			{
				result->setSuccess(resultObj[i].value_.get_bool());
			}
			else if(resultObj[i].name_ == L"message" && (!resultObj[i].value_.is_null()))
			{
				if(result->getSuccess())
				{
					// in success case message is an empty array -> we do nothing
				}
				else
				{
					// parse the error string
					result->setMessage(resultObj[i].value_.get_str());
				}
			}
			else if(resultObj[i].name_ == L"data")
			{
				// Currently data is not used
				json_spirit::wObject dataObj = resultObj[i].value_.get_obj();

				for (size_t idata = 0; idata < dataObj.size(); idata++) 
				{
				}
			}
		}
	}

	return result;
}

std::vector<wstring> ObjectConverter::GetCustomerList(const wstring& json)
{
	std::vector<wstring> vResult;

	// Parse the string to a json value
	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		// Get the json object
		json_spirit::wObject jsonObj = value.get_obj();
		for (size_t j=0; j < jsonObj.size(); j++)
		{
			if(jsonObj[j].name_ == L"topics")
			{
				json_spirit::wArray topics = jsonObj[j].value_.get_array();
				for (size_t i = 0; i < topics.size(); i++)
				{
					// An a single array element
					json_spirit::wObject arrElement = topics[i].get_obj();

					wstring companyName;
					wstring city;
					wstring zip;
					wstring id;
					wstring secretKey;
					wstring customerNr;

					for(size_t iEle = 0; iEle < arrElement.size(); iEle++)
					{
						// Read the single elements attributes
						if(arrElement[iEle].name_ == L"companyName")
						{
							companyName = arrElement[iEle].value_.get_str();
						}
						else if(arrElement[iEle].name_ == L"zipCode")
						{
							zip = arrElement[iEle].value_.get_str();
						}
						else if(arrElement[iEle].name_ == L"city")
						{
							city = arrElement[iEle].value_.get_str();
						}
						else if(arrElement[iEle].name_ == L"id")
						{
							id = arrElement[iEle].value_.get_str();
						}
						else if(arrElement[iEle].name_ == L"secretKey")
						{
							secretKey = arrElement[iEle].value_.get_str();
						}
						else if (arrElement[iEle].name_ == L"customerNr")
						{
							customerNr = StringConverter::decimal2String(arrElement[iEle].value_.get_int());
						}
					}
					// Add customer to vec
					vResult.push_back(companyName + L";" + zip + L";" + city + L";" + id + L";" + secretKey + L";" + customerNr + L";");
				}
			}
		}
	}

	return vResult;
}

CreateCustomerResultPtr ObjectConverter::GetCreateCustomerResult(const wstring& json) 
{
	CreateCustomerResultDataPtr result(new CreateCustomerResultData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		// Get the json object
		json_spirit::wObject jsonObj = value.get_obj();
		for (size_t j=0; j < jsonObj.size(); j++)
		{
			if(jsonObj[j].name_ == L"created")
			{
				result->setIsCreated( jsonObj[j].value_.get_bool() );
			}
			else if(jsonObj[j].name_ == L"msg")
			{
				result->setMessage( jsonObj[j].value_.get_str() );
			}
		}
	}

	return result;
}

CustomerPtr ObjectConverter::getCustomerByEmail(const wstring& json)
{
	CustomerDataPtr customer(new CustomerData());

	json_spirit::wValue value;
	if(json_spirit::read(json, value))
	{
		json_spirit::wObject jsonObj = value.get_obj();
		for (size_t i = 0; i < jsonObj.size(); i++)
		{
			// Get the json object
			if(jsonObj[i].name_ == L"customer")
			{
				// Get customer data
				json_spirit::wObject customerObj = jsonObj[i].value_.get_obj();
				for (size_t j = 0; j < customerObj.size(); j++)
				{
					if(customerObj[j].name_ == L"phone")
					{
						customer->setPhone( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"prename")
					{
						customer->setSurname( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"street")
					{
						customer->setStreet( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"surname")
					{
						customer->setSurname( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"companyName")
					{
						customer->setCompany( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"city")
					{
						customer->setCity( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"country")
					{
						customer->setCountry( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"id")
					{
						customer->setId( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"customerNr")
					{
						customer->setCustomerNr( customerObj[j].value_.get_int() );
					}
					else if(customerObj[j].name_ == L"email")
					{
						customer->setEmail( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"zipCode")
					{
						customer->setZipCode( customerObj[j].value_.get_str() );
					}
					else if(customerObj[j].name_ == L"oceNr")
					{
						customer->setOceNumber( customerObj[j].value_.get_int() );
					}
					else if(customerObj[j].name_ == L"deactivated")
					{
						customer->setDeactivated( customerObj[j].value_.get_bool() );
					}
					else if(customerObj[j].name_ == L"streetNumber")
					{
						customer->setStreetNumber( customerObj[j].value_.get_str() );
					}
				}
			}
		}	
		return customer;
	}
	return nullptr;
}


bool ObjectConverter::IsValidJSON(const wstring& json)
{
	json_spirit::wValue value;
	return json_spirit::read(json, value);
}

