#ifndef __HTML_H__
#define __HTML_H__

class HTMLTag{
private:
	char *tag;
	char *fulltag;
	char *text;
	HTMLTag *next;
	HTMLTag *parent;
public:
	HTMLTag(char *pp, char *pn);
	~HTMLTag();
	HTMLTag* GetNext();
	HTMLTag* GetParent();
	void SetNext(HTMLTag *ht);
	void SetParent(HTMLTag *ht);
	char* GetTag();
	char* GetFullTag();
	bool Get(char *name, char *out);
	void SetText(char *s);
	char* GetText();
};

class HTMLParser{
private:
	char *html;
	HTMLTag *hti;
	HTMLTag *htlist;
	HTMLTag *htp;
	void Search(int d = 0);
public:
	HTMLParser(char *src);
	~HTMLParser();
	void ShowTree();
	HTMLTag* Find(char *path, int d = 0);
	HTMLTag * GetTagList();
};

#endif