"""

Filter field by field by applying one or several filters
on target string like account@domain@device:service

For active the target accurate filter mode, user MUST type on
selector research a string form beginning by '$' character like : 
"$keyword1:value1,$keyword2:value2,$keyword3:value3"

Each wanted filter must following the "$keyword:value" 
format for be applicable.

For example, the following target filter string will filter on
account, domain, device and service fields 
according to the passed values : 
"$account:my_account,$domain:my_domain,$device:my_device,$service:my_service"
    
We can combine 2 filters or more like above but we 
can also execute only one filter on target.
The following target filter strings are accepted :
"$account:my_account"
"$domain:my_domain"
"$device:my_device"
"$service:my_service"

There is no specifc order for apply filters. 
The following target filter string is also valid :
"$device:my_device,$account:my_account,$domain:my_domain"

However, target filter string badly formatted like containing 
duplicated filter keywords, missing values and others aren't 
allowed and will be skipped and considered as no valid.
The following examples are forbidden :
"$account:my_account,$account:my_account"
"$domain:my_domain,"
"$my_device:my_device"
"$service:"

"""


import re

class FilterKeywordSyntaxError(RuntimeError) :
    def __init__(self, keyword) :
        super().__init__(
            self, ("unknown filter keyword with '%s'"
                   % keyword))

class FilterKeywordDuplicateError(RuntimeError) :
    def __init__(self, keyword) :
        super().__init__(
            self, ("duplicated filter keyword with '%s'"
                   % keyword))

class ParsingError(RuntimeError) :
    def __init__(self, bad_string) :
        super().__init__(
            self, ("'keyword:value' format parsing error with '%s'"
                   % bad_string))



DEVI_SERV_SEP_REG = re.compile("[:+]")

def get_fields_from_devi_serv(devi_serv) :
    global DEVI_SERV_SEP_REG
    
    token_list = re.split(DEVI_SERV_SEP_REG, devi_serv)
    token_count = len(token_list)
    field_dict = {}
    
    if token_count == 1 : # devi_serv string like : device
        field_dict.update(
            {"device" : devi_serv})
    elif token_count == 2 : # devi_serv string like : device:service (or device+service)
        device, service = token_list
 
        field_dict.update(
            {"device" : device,
             "service" : service})
    else :
        raise ValueError("device:service string badly formatted : '%s'"
                         % devi_serv)
    return field_dict

        
TARGET_SEP_REG = re.compile("[@]")
    
def get_target_field_dict(target) :
    global TARGET_SEP_REG
    
    token_list = re.split(TARGET_SEP_REG, target)
    token_count = len(token_list)
    field_dict = {}
    
    if token_count == 2 : # target string like : account@device
        account, devi_serv = token_list
    
        field_dict.update(
            {"account" : account})
    elif token_count == 3 : # target string like : account@domain@device
        account, domain, devi_serv = token_list

        field_dict.update(
            {"account" : account,
             "domain" : domain})
    else :
        raise ValueError("target string badly formatted : '%s'" % target)
    field_dict.update(get_fields_from_devi_serv(devi_serv))
    return field_dict



def is_filter_keyword(filter_keyword) :
    if filter_keyword.startswith('$') :
        filter_keyword_no_prefix = filter_keyword[1:]
        
        if (filter_keyword_no_prefix in ("account",
                                         "domain",
                                         "device",
                                         "service")) :
            return True, filter_keyword_no_prefix
        return False, None

def is_filterable(filter_patterns, target_field_dict) :
    FILTER_SEPARATOR = ','
    FILTER_KV_SEPARATOR = ':'
    filter_pattern_dict = {}

    for filter_kv in filter_patterns.split(FILTER_SEPARATOR) :
        token_list = filter_kv.split(FILTER_KV_SEPARATOR, 1)

        if len(token_list) != 2 :
            raise ParsingError(filter_kv)
        
        filter_keyword, filter_value = token_list
        
        if not filter_value :
            raise ParsingError(filter_kv)
    
        res, filter_keyword_no_prefix = is_filter_keyword(filter_keyword)
        
        if not res :
            raise FilterKeywordSyntaxError(filter_keyword)
        if filter_keyword_no_prefix not in filter_pattern_dict :
            filter_pattern_dict[
                filter_keyword_no_prefix] = filter_value
        else :
            raise FilterKeywordDuplicateError(filter_keyword)

    filter_is_applicable = lambda fk, fv, d : fk in d and fv in d[fk]

    assert target_field_dict, "target_field_dict is empty"
    return all(filter_is_applicable(filter_keyword,
                                    filter_value,
                                    target_field_dict)
               for filter_keyword, filter_value
               in filter_pattern_dict.items())

