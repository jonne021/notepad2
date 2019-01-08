// Scintilla source code edit control
/** @file RESearch.h
 ** Interface to the regular expression search library.
 **/
// Written by Neil Hodgson <neilh@scintilla.org>
// Based on the work of Ozan S. Yigit.
// This file is in the public domain.

#ifndef RESEARCH_H
#define RESEARCH_H

namespace Scintilla {

/*
 * The following defines are not meant to be changeable.
 * They are for readability only.
 */
#define MAXCHR	256
#define CHRBIT	8
#define BITBLK	(MAXCHR/CHRBIT)

class CharacterIndexer {
public:
	virtual char CharAt(Sci::Position index) const noexcept = 0;
	virtual ~CharacterIndexer() = default;
};

class RESearch {

public:
	explicit RESearch(const CharClassify *charClassTable);
	// No dynamic allocation so default copy constructor and assignment operator are OK.
	~RESearch();
	void Clear() noexcept;
	void GrabMatches(const CharacterIndexer &ci);
	const char *Compile(const char *pattern, Sci::Position length, bool caseSensitive, int flags) noexcept;
	int Execute(const CharacterIndexer &ci, Sci::Position lp, Sci::Position endp);

	enum {
		MAXTAG = 10
	};
	enum {
		MAXNFA = 4096
	};
	enum {
		NOTFOUND = -1
	};

	Sci::Position bopat[MAXTAG];
	Sci::Position eopat[MAXTAG];
	std::string pat[MAXTAG];

private:
	void ChSet(unsigned char c) noexcept;
	void ChSetWithCase(unsigned char c, bool caseSensitive) noexcept;
	int GetBackslashExpression(const char *pattern, int &incr) noexcept;

	const char *DoCompile(const char *pattern, Sci::Position length, bool caseSensitive, bool posix) noexcept;
	Sci::Position PMatch(const CharacterIndexer &ci, Sci::Position lp, Sci::Position endp, char *ap);

	Sci::Position bol;
	Sci::Position tagstk[MAXTAG];  /* subpat tag stack */
	char nfa[MAXNFA];    /* automaton */
	int sta;
	int failure;

	// cache for previous pattern with same address, length and flags
	const char *previousPattern;
	Sci::Position previousLength;
	int previousFlags;

	unsigned char bittab[BITBLK]; /* bit table for CCL pre-set bits */
	const CharClassify *charClass;
	bool iswordc(unsigned char x) const noexcept {
		return charClass->IsWord(x);
	}
};

}

#endif

