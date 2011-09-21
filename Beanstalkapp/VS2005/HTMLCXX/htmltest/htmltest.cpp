// htmltest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../htmlcxx-0.84/html/ParserDom.h"
#include "../htmlcxx-0.84/html/utils.h"
#include "../htmlcxx-0.84/html/wincstring.h"

using namespace htmlcxx;

int _tmain(int argc, _TCHAR* argv[])
{
	std::string						strhtml		= "<html>"
		"<head>"
		"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />"
		"<meta name=\"keywords\" content=\"key1, key2, key3\" />"
		"<meta name=\"filter\" content=\"sample\" />"
		"<meta name=\"path\" content=\"/sample\" />"
		"<meta name=\"location\" content=\"location\" />"
		"<Title>札记样本</title>"
		"</head>"
		"<body style=\"\">"
		"<div>This <p>is</p> a test</div>"
		"</body>"
		"</html>";
	HTML::ParserDom						html;
	tree<HTML::Node>::iterator			node;

	html.parseTree(strhtml);
	for(node = html.getTree().begin()
		; node != html.getTree().end()
		; node++)
	{
		std::string			tagName		= node->tagName();

		if(!node->isTag())
			continue;

		if(strcasecmp("meta", tagName.c_str()) == 0)
		{
			std::map<std::string, std::string>::const_iterator	name;
			std::map<std::string, std::string>::const_iterator	content;

			node->parseAttributes();
			name = node->attributes().find("name");
			if(node->attributes().end() != name)
			{
				content = node->attributes().find("content");
				if(node->attributes().end() == content)
					continue;
				printf("%s: %s\r\n", name->second.c_str(), content->second.c_str());
			}
		}
		else if(strcasecmp("title", tagName.c_str()) == 0 || strcasecmp("body", tagName.c_str()) == 0)
		{
			std::string					strtext;
			int							nLen			= 0;
			int							nPos			= 0;

			nPos = node->offset() + strlen(node->text().c_str());
			nLen = node->length() - strlen(node->text().c_str()) - strlen(node->closingText().c_str());
			if(nLen < 0)
				continue;
			strtext.append(strhtml.c_str() + nPos, nLen);
			printf("%s: %s\r\n", node->tagName().c_str(), strtext.c_str());
		}
	}

	system("Pause");
	return 0;
}

