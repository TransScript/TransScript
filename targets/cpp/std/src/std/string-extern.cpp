// Copyright (c) 2016 IBM Corporation.

#include "string-extern.h"

#include "std/core.h"
#include "std/listdef.h"
#include "strutils.h"
#include <memory.h>
#include <regex>

using namespace tosca;

StringTerm& AfterFirst(Context& ctx, StringTerm& string, StringTerm& sep)
{
    const tosca::string& ustring = string.Unbox();
    const tosca::string& usep = sep.Unbox();
    tosca::string::size_type idx = ustring.find(usep);

    StringTerm& result = newStringTerm(ctx, (idx == tosca::string::npos) ? "" : ustring.substr(idx + 1).c_str());
    string.Release();
    sep.Release();
    return result;
}

StringTerm& BeforeFirst(Context& ctx, StringTerm& string, StringTerm& sep)
{
    const tosca::string& ustring = string.Unbox();
    const tosca::string& usep = sep.Unbox();
    tosca::string::size_type idx = ustring.find(usep);

    sep.Release();

    if (idx == tosca::string::npos)
    {
        return string;
    }
    else
    {
        char *c_ubefore = (char*) malloc(idx+1);
        memcpy(c_ubefore, ustring.c_str(), idx);
        c_ubefore[idx] = '\0';
        StringTerm& result = newStringTerm(ctx, (const char *)c_ubefore);
        free(c_ubefore);
        string.Release();

        return result;
    }
}

Bool& StringEqual(Context& ctx, StringTerm& str1, StringTerm& str2)
{
    const tosca::string& ustr1 = str1.Unbox();
    const tosca::string& ustr2 = str2.Unbox();
    Bool& result = ustr1 == ustr2 ? newTRUE(ctx) : newFALSE(ctx);
    str1.Release();
    str2.Release();
    return result;
}

StringTerm& Escape(Context& ctx, StringTerm& str)
{
    StringTerm& result = newStringTerm(ctx, makeEscaped(str.Unbox()));
    str.Release();
    return result;
}

DoubleTerm& Length(Context& ctx, StringTerm& str)
{
    DoubleTerm& result = newDoubleTerm(ctx, str.Unbox().size());
    str.Release();
    return result;
}

StringTerm& Mangle(Context& ctx, StringTerm& str)
{
    StringTerm& result = newStringTerm(ctx, makeMangle(str.Unbox()));
    str.Release();
    return result;
}

StringTerm& UpCase(Context& ctx, StringTerm& str)
{
    tosca::string upper(str.Unbox());
    for (tosca::string::iterator it= upper.begin(); it != upper.end(); ++it)
        *it = toupper(*it);
    str.Release();
    return newStringTerm(ctx, upper.c_str());
}

StringTerm& DownCase(Context& ctx, StringTerm& str)
{
    tosca::string lower(str.Unbox());
    for (tosca::string::iterator it= lower.begin(); it != lower.end(); ++it)
        *it = tolower(*it);
    str.Release();
    return newStringTerm(ctx, lower.c_str());
}

StringTerm& Replace(Context& ctx, StringTerm& str, StringTerm& oldStr, StringTerm& newStr)
{
    if (oldStr.Unbox().empty())
    {
        oldStr.Release();
        newStr.Release();
        return str;
    }

    tosca::string result(str.Unbox());
    const tosca::string& uoldStr = oldStr.Unbox();
    const tosca::string& unewStr = newStr.Unbox();

    size_t pos = 0;
    while ((pos = result.find(uoldStr, pos)) != tosca::string::npos)
    {
        result.replace(pos, uoldStr.length(), unewStr);
        pos += unewStr.length();
    }
    str.Release();
    oldStr.Release();
    newStr.Release();

    return newStringTerm(ctx, result.c_str());
}

Bool& Contains(Context& ctx, StringTerm& str1, StringTerm& str2)
{
    const tosca::string& ustr1 = str1.Unbox();
    const tosca::string& ustr2 = str2.Unbox();
    Bool& result = ustr1.find(ustr2) != tosca::string::npos ? newTRUE(ctx) : newFALSE(ctx);
    str1.Release();
    str2.Release();
    return result;
}


StringTerm& Substring(Context& ctx, StringTerm& str, DoubleTerm& from, DoubleTerm& to)
{
    const tosca::string& ustr = str.Unbox();
    tosca::string::size_type pos = static_cast<tosca::string::size_type>(from.Unbox());
    tosca::string::size_type end = static_cast<tosca::string::size_type>(to.Unbox());
    tosca::string::size_type count = end > pos ? end - pos : 0;
    StringTerm& result = newStringTerm(ctx, ustr.substr(pos, count).c_str());
    str.Release();
    from.Release();
    return result;
}

StringTerm& Substring2(Context& ctx, StringTerm& str, DoubleTerm& from)
{
    const tosca::string& ustr = str.Unbox();
    tosca::string::size_type pos = static_cast<tosca::string::size_type>(from.Unbox());
    StringTerm& result = newStringTerm(ctx, ustr.substr(pos).c_str());
    str.Release();
    from.Release();
    return result;
}

#ifdef STD_REGEX
Bool& MatchRegex(Context& ctx, StringTerm& pattern, StringTerm& str)
{
    std::regex regex(pattern.Unbox());
    Bool& result = std::regex_match(str.Unbox(), regex) ? newTRUE(ctx) : newFALSE(ctx);
    pattern.Release();
    str.Release();
    return result;
}
#endif

Bool& StartsWith(Context& ctx, StringTerm& str, StringTerm& prefix)
{
    const tosca::string& ustr = str.Unbox();
    const tosca::string& uprefix = prefix.Unbox();
    Bool& result = (!ustr.compare(0, uprefix.size(), uprefix)) ? newTRUE(ctx) : newFALSE(ctx);
    str.Release();
    prefix.Release();
    return result;
}

Bool& EndsWith(tosca::Context& ctx, tosca::StringTerm& str, tosca::StringTerm& suffix)
{
    const tosca::string& ustr = str.Unbox();
    const tosca::string& usuffix = suffix.Unbox();
    Bool& result = (ustr.size() >= usuffix.size()
                    && !ustr.compare(ustr.size() - usuffix.size(), usuffix.size(), usuffix)) ? newTRUE(ctx) : newFALSE(ctx);
    str.Release();
    suffix.Release();
    return result;
}

tosca::StringTerm& Trim(tosca::Context& ctx, tosca::StringTerm& str)
{
    const tosca::string& ustr = str.Unbox();
    if (ustr.empty())
        return str;

    tosca::string::size_type first = ustr.find_first_not_of(" \t\f\n\r\b");
    if (first == tosca::string::npos)
    {
        // All whitespace characters.
        str.Release();
        return newStringTerm(ctx, "");
    }

    size_t last = ustr.find_last_not_of(" \t\f\n\r\b");
    StringTerm& result = newStringTerm(ctx, ustr.substr(first, (last-first+1)).c_str());
    str.Release();
    return result;
}

List<tosca::StringTerm>& Split(tosca::Context& ctx, tosca::StringTerm& str, tosca::StringTerm& sep)
{
    const tosca::string& ustr = str.Unbox();
    if (ustr.empty())
    {
        str.Release();
        sep.Release();
        return newNil<tosca::StringTerm>(ctx);
    }

    const tosca::string& usep = sep.Unbox();
    List<tosca::StringTerm>* result = 0;
    List<tosca::StringTerm>* last = 0;

    tosca::string::size_type spos = 0;
    tosca::string::size_type pos = 0;
    int trailings = 0; // to discard empty trailing strings
    while ((pos = ustr.find(usep, spos)) != tosca::string::npos)
    {
    	tosca::string::size_type count = pos - spos;
    	if (count == 0)
    	{
    		trailings ++;
    		spos += usep.length();
    		continue;
    	}
    	for (; trailings > 0; trailings--)
    	{
    		List<tosca::StringTerm>& cons = dynamic_cast<List<tosca::StringTerm>&>(_CCons<tosca::StringTerm>::Make(ctx));
    		cons.SetSub(0, newStringTerm(ctx, ""));
    		if (last)
    			last->SetSub(1, cons);
    		last = &cons;

    		if (!result)
    			result = last;
    	}

    	List<tosca::StringTerm>& cons = dynamic_cast<List<tosca::StringTerm>&>(_CCons<tosca::StringTerm>::Make(ctx));
    	cons.SetSub(0, newStringTerm(ctx, ustr.substr(spos, count).c_str()));
    	if (last)
    		last->SetSub(1, cons);
    	last = &cons;

    	if (!result)
    		result = last;

        spos = pos + usep.length();
    }
    if (spos < ustr.length())
    {
    	List<tosca::StringTerm>& cons = dynamic_cast<List<tosca::StringTerm>&>(_CCons<tosca::StringTerm>::Make(ctx));
    	cons.SetSub(0, newStringTerm(ctx, ustr.substr(spos).c_str()));
    	if (last)
    		last->SetSub(1, cons);
    	last = &cons;

    	if (!result)
    		result = last;
    }

    if (last)
    {
        last->SetSub(1, newNil<tosca::StringTerm>(ctx));
        str.Release();
    }
    else
    {
        // sep was not found or only trailing empty strings. Just return the original string, even when empty
        result = &newCons(ctx, str, newNil<tosca::StringTerm>(ctx));
    }
    sep.Release();
    return *result;
}

tosca::StringTerm& Squash(tosca::Context& ctx, tosca::StringTerm& str)
{
    tosca::string squashed;
    bool wasspace = false;
    
    for (auto iter = str.Unbox().begin(); iter != str.Unbox().end(); iter++)
    {
        if (isspace(*iter))
        {
            if (!wasspace)
            {
                wasspace=true;
                squashed += ' ';
            }
        }
        else
        {
            squashed += *iter;
            wasspace = false;
        }
    }
    str.Release();
    return newStringTerm(ctx, squashed.c_str());
}

tosca::DoubleTerm& Index(tosca::Context& ctx, tosca::StringTerm& string, tosca::StringTerm& pattern)
{
    auto search = string.Unbox().find(pattern.Unbox());
    tosca::DoubleTerm& result = newDoubleTerm(ctx, search == tosca::string::npos ? -1 : search);
    string.Release();
    pattern.Release();
    return result;
}

