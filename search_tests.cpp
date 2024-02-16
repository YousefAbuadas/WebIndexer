#include <gtest/gtest.h>

#include "search.h"
#include "tests/build_index.h"
#include "tests/clean_token.h"
#include "tests/find_query_matches.h"
#include "tests/gather_tokens.h"


/*
**TESTS FOR CleanToken**
*/
//tests empty/space tokens
TEST(CleanToken, ExampleEmptyTest) {
    map<string, set<string>> tests;
    tests[""].insert("");

    testCleanToken(tests);
}

//tests tokens punctuations in front
TEST(CleanToken, PunctuationInFront) {
    map<string, set<string>> tests;
    tests["hello"].insert("!hello");
    tests["hello"].insert("??hello");
    tests["hello"].insert(".....hello");

    testCleanToken(tests);
}

//tests tokens punctuations in back
TEST(CleanToken, PunctuationInBack) {
    map<string, set<string>> tests;
    tests["hello"].insert("hello!");
    tests["hello"].insert("hello??");
    tests["hello"].insert("hello......");

    testCleanToken(tests);
}

//tests tokens punctuations in front and back
TEST(CleanToken, PunctuationFrontAndBack) {
    map<string, set<string>> tests;
    tests["hello"].insert("!hello!");
    tests["hello"].insert("??hello??");
    tests["hello"].insert("......hello.......");

    testCleanToken(tests);
}

//tests when token has no alphabet letters
TEST(CleanToken, NoAlphabetLetters) {
    map<string, set<string>> tests;
    tests[""].insert("123456");
    tests[""].insert("!!!???");
    tests[""].insert("1234!@#$");

    testCleanToken(tests);
}

//tests when token has mixed alphabet letters
TEST(CleanToken, MixedAlphabetLetters) {
    map<string, set<string>> tests;
    tests["hello"].insert("HeLlO");
    tests["hello"].insert("hElLo");
    tests["hello"].insert("HELLo");

    testCleanToken(tests);
}

//tokens punctuation in random places
TEST(CleanToken, PunctuationRandomPlaces) {
    map<string, set<string>> tests;
    tests["he.l!lo"].insert("he.l!lo");
    tests["he,l?lo"].insert("he,l?lo");
    tests["h.e.l.l.o"].insert(".h.e.l.l.o.");
    tests["hel...lo"].insert("...hel...lo");
    tests["hel...lo"].insert("hel...lo...");

    testCleanToken(tests);
}

//tests token punctuations in middle
TEST(CleanToken, PunctuationInMiddle) {
    map<string, set<string>> tests;
    tests["he,llo"].insert("he,llo");
    tests["he...llo"].insert("he...llo");
    tests["hel-----lo"].insert("hel-----lo");

    testCleanToken(tests);
}

//tests token numbers 
TEST(CleanToken, NumericalCharacters) {
    map<string, set<string>> tests;
    tests["hello2"].insert("hello2");
    tests["123world"].insert("123world");
    tests["example123"].insert("example123");

    testCleanToken(tests);
}

//tests whitespace tokens
TEST(CleanToken, WhitespaceHandling) {
    map<string, set<string>> tests;
    tests[""].insert(" ");
    tests[""].insert("\t");
    tests["hello   world"].insert("hello   world");
    tests["   hello   world   "].insert("   hello   world   ");

    testCleanToken(tests);
}

/*
**TESTS FOR GatherTokens**
*/

//tests for leading space in text
TEST(GatherTokens, LeadingSpace) {
    string text = " leading space in text";
    set<string> expected = {"leading", "space", "in", "text"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

//tests for trailing space in text
TEST(GatherTokens, TrailingSpace) {
    string text = "trailing space in text ";
    set<string> expected = {"trailing", "space", "in", "text"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

//tests for multiple spaces between words in text
TEST(GatherTokens, MultipleSpaces) {
    string text = "multiple  spaces   between    words";
    set<string> expected = {"multiple", "spaces", "between", "words"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

//tests for multiple of same words in text
TEST(GatherTokens, RepeatedWords) {
    string text = "repeat repeat repeat the word repeat";
    set<string> expected = {"repeat", "the", "word"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

//tests for uppercase and lowercase of the same word in text
TEST(GatherTokens, CaseSensitivity) {
    string text = "Case and case and CASE";
    set<string> expected = {"case", "and"}; 
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}


//tests for empty text
TEST(GatherTokens, EmptyText) {
    string text = "";
    set<string> expected = {};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

//tests for punctuation within and outside the word
TEST(GatherTokens, PunctuationHandling) {
    string text = "punctuation, inside and outside: word; another-word!";
    set<string> expected = {"punctuation", "inside", "and", "outside", "word", "another-word"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

//tests for unique case where cleaned token is the same as another word
TEST(GatherTokens, CleanedToken) {
    string text = "This is a cleaned token ...TOKEN...";
    set<string> expected = { "a", "cleaned", "is", "this", "token"};
    set<string> studentResult = gatherTokens(text);

    string testFeedback = "gatherTokens(\"" + text + "\") incorrect\n";

    EXPECT_EQ(expected, studentResult) << testFeedback;
}

/*
**TESTS FOR BuildIndex**
*/

//tests when file doesnt open
TEST(BuildIndex, FileDoesNotOpen) {
    string filename = "nonexistent_file.txt";
    map<string, set<string>> studentIndex;
    int studentNumProcessed = buildIndex(filename, studentIndex);

    EXPECT_EQ(true, studentIndex.empty()) << "Index should be empty if the file cannot be opened";

    EXPECT_EQ(0, studentNumProcessed) << "Number of processed URLs should be 0 if the file cannot be opened";
}

//tests when a cleaned word is the same as another word in a different website
TEST(BuildIndex, HandleWordsInMultipleSites) {
    string filename = "tiny.txt";  
    map<string, set<string>> expected = {
        {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
        {"eggs", {"www.shoppinglist.com"}},
        {"milk", {"www.shoppinglist.com"}},
        {"bread", {"www.shoppinglist.com"}},
        {"cheese", {"www.shoppinglist.com"}},
        {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
        {"green", {"www.rainbow.org"}},
        {"orange", {"www.rainbow.org"}},
        {"yellow", {"www.rainbow.org"}},
        {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
        {"indigo", {"www.rainbow.org"}},
        {"violet", {"www.rainbow.org"}},
        {"one", {"www.dr.seuss.net"}},
        {"two", {"www.dr.seuss.net"}},
        {"i'm", {"www.bigbadwolf.com"}},
        {"not", {"www.bigbadwolf.com"}},
        {"trying", {"www.bigbadwolf.com"}},
        {"to", {"www.bigbadwolf.com"}},
        {"eat", {"www.bigbadwolf.com"}},
        {"you", {"www.bigbadwolf.com"}}
    };

    map<string, set<string>> studentIndex;
    int studentNumProcessed = buildIndex(filename, studentIndex);

    EXPECT_EQ(expected, studentIndex) << "buildIndex did not correctly handle words appearing in multiple sites";

    string retTestFeedback = "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
    EXPECT_EQ(4, studentNumProcessed) << retTestFeedback;
}

/*
**TESTS FOR FindQueryMatches**
*/

//tests for empty sentence as a parameter
TEST(FindQueryMatches, EmptyQuery) {
    set<string> expected = {};
    EXPECT_EQ(expected, findQueryMatches(INDEX, "")) << "Empty query should return no matches";
}


//tests for first word not in map
TEST(FindQueryMatches, FirstWordNotInMap) {
    set<string> expected = {"example.com", "uic.edu"}; 
    EXPECT_EQ(expected, findQueryMatches(INDEX, "test hello")) << "Query with first word not in index should return no matches";
}


//tests for later word with + modifier not in map
TEST(FindQueryMatches, PlusModifierWordNotInMap) {
    set<string> expected = {}; 
    EXPECT_EQ(expected, findQueryMatches(INDEX, "hello +test")) << "Query with a required word not in index should return no matches";
}


//tests for later word with - modifier not in map
TEST(FindQueryMatches, MinusModifierWordNotInMap) {
    set<string> expected = {"example.com", "uic.edu"}; 
    EXPECT_EQ(expected, findQueryMatches(INDEX, "hello -test")) << "Query with a non existent exclusion word should not affect results";
}


//tests for later word with no modifier not in map
TEST(FindQueryMatches, OptionalWordNotInMap) {
    set<string> expected = {"example.com", "uic.edu"}; 
    EXPECT_EQ(expected, findQueryMatches(INDEX, "hello test")) << "Query with an optional word not in index should return matches for other words";
}
