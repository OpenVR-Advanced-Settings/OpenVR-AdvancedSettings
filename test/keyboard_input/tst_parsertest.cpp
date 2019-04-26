#include <QtTest>
#include <QDebug>
#include "input_parser.h"

class ParserTest : public QObject
{
    Q_OBJECT

private slots:
    void singleCharacters();
    void singleCharactersShouldFail();

    void severalCharacters();
    void severalCharactersShouldFail();

    void singleDigit();
    void singleDigitShouldFail();

    void severalDigits();
    void severalDigitsShouldFail();

    void Modifiers();

    void functionKeys();

    void shouldExitEarlyFunctionKey();

    void releaseHeldKeys();

    void capitalLiterals();

    void abortedCapitalLiterals();

    void multipleCapitalLiterals();

    void sixtyfourBackspacesBenchmarked();

    void sixtyfourAsBenchmarked();

    void removeDuplicateSpaces();

    void removeDuplicateModifiers();

    void removeIncorrectTokensBenchmark();
};

const std::string alphabet = "abcdefghijklmnopqrstuvxyz";
const std::string digits = "0123456789";

void ParserTest::singleCharacters()
{
    for ( const auto letter : alphabet )
    {
        const auto l = std::string( &letter );
        const auto t = std::vector<Token>( { static_cast<Token>( letter ) } );
        QCOMPARE( ParseKeyboardInputsToTokens( l ), t );
    }
}

void ParserTest::singleCharactersShouldFail()
{
    for ( const auto letter : alphabet )
    {
        const auto l = std::string( &letter );
        const auto t
            = std::vector<Token>( { static_cast<Token>( letter + 1 ) } );
        QVERIFY( ParseKeyboardInputsToTokens( l ) != t );
    }
}

void ParserTest::severalCharacters()
{
    const auto tokens = ParseKeyboardInputsToTokens( "mad" );
    const auto e
        = std::vector<Token>( { Token::KEY_m, Token::KEY_a, Token::KEY_d } );
    QCOMPARE( tokens, e );
}

void ParserTest::severalCharactersShouldFail()
{
    const auto tokens = ParseKeyboardInputsToTokens( "mad" );
    const auto e
        = std::vector<Token>( { Token::KEY_m, Token::KEY_e, Token::KEY_d } );
    QVERIFY( tokens != e );
}

void ParserTest::singleDigit()
{
    for ( const auto n : digits )
    {
        const auto l = std::string( &n );
        const auto t = std::vector<Token>( { static_cast<Token>( n ) } );
        QCOMPARE( ParseKeyboardInputsToTokens( l ), t );
    }
}

void ParserTest::singleDigitShouldFail()
{
    for ( const auto n : digits )
    {
        const auto l = std::string( &n );
        const auto t = std::vector<Token>( { static_cast<Token>( n + 1 ) } );
        QVERIFY( ParseKeyboardInputsToTokens( l ) != t );
    }
}

void ParserTest::severalDigits()
{
    const auto t = ParseKeyboardInputsToTokens( "123" );
    const auto e
        = std::vector<Token>( { Token::KEY_1, Token::KEY_2, Token::KEY_3 } );
    QVERIFY( t == e );
}

void ParserTest::severalDigitsShouldFail()
{
    const auto t = ParseKeyboardInputsToTokens( "123" );
    const auto e
        = std::vector<Token>( { Token::KEY_1, Token::KEY_3, Token::KEY_2 } );
    QVERIFY( t != e );
}

void ParserTest::Modifiers()
{
    auto t = ParseKeyboardInputsToTokens( "^" );
    auto e = std::vector<Token>( { Token::MODIFIER_CTRL } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "*" );
    e = std::vector<Token>( { Token::MODIFIER_ALT } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( ">" );
    e = std::vector<Token>( { Token::MODIFIER_SHIFT } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "#" );
    e = std::vector<Token>( { Token::MODIFIER_SUPER } );
    QVERIFY( t == e );
}

void ParserTest::functionKeys()
{
    auto t = ParseKeyboardInputsToTokens( "F1" );
    auto e = std::vector<Token>( { Token::KEY_F1 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F2" );
    e = std::vector<Token>( { Token::KEY_F2 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F3" );
    e = std::vector<Token>( { Token::KEY_F3 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F4" );
    e = std::vector<Token>( { Token::KEY_F4 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F5" );
    e = std::vector<Token>( { Token::KEY_F5 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F6" );
    e = std::vector<Token>( { Token::KEY_F6 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F7" );
    e = std::vector<Token>( { Token::KEY_F7 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F8" );
    e = std::vector<Token>( { Token::KEY_F8 } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F9" );
    e = std::vector<Token>( { Token::KEY_F9 } );
    QVERIFY( t == e );
}

void ParserTest::shouldExitEarlyFunctionKey()
{
    auto t = ParseKeyboardInputsToTokens( "asF" );
    auto e = std::vector<Token>( { Token::KEY_a, Token::KEY_s } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "asFs" );
    e = std::vector<Token>( { Token::KEY_a, Token::KEY_s } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F" );
    e = std::vector<Token>( {} );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "Fsdfg" );
    e = std::vector<Token>( {} );
    QVERIFY( t == e );
}

void ParserTest::releaseHeldKeys()
{
    auto t = ParseKeyboardInputsToTokens( "a s" );
    auto e = std::vector<Token>(
        { Token::KEY_a, Token::TOKEN_NEW_SEQUENCE, Token::KEY_s } );
    QVERIFY( t == e );
}

void ParserTest::capitalLiterals()
{
    auto t = ParseKeyboardInputsToTokens( "BACKSPACE" );
    auto e = std::vector<Token>( { Token::KEY_BACKSPACE } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "SPACE" );
    e = std::vector<Token>( { Token::KEY_SPACE } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "TAB" );
    e = std::vector<Token>( { Token::KEY_TAB } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "ESC" );
    e = std::vector<Token>( { Token::KEY_ESC } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "INS" );
    e = std::vector<Token>( { Token::KEY_INS } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "DEL" );
    e = std::vector<Token>( { Token::KEY_DEL } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "END" );
    e = std::vector<Token>( { Token::KEY_END } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "PGDN" );
    e = std::vector<Token>( { Token::KEY_PGDN } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "PGUP" );
    e = std::vector<Token>( { Token::KEY_PGUP } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "CAPS" );
    e = std::vector<Token>( { Token::KEY_CAPS } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "PRNSCRN" );
    e = std::vector<Token>( { Token::KEY_PRNSCRN } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "PAUSE" );
    e = std::vector<Token>( { Token::KEY_PAUSE } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "SCRLOCK" );
    e = std::vector<Token>( { Token::KEY_SCRLOCK } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "LEFTARROW" );
    e = std::vector<Token>( { Token::KEY_LEFTARROW } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "RIGHTARROW" );
    e = std::vector<Token>( { Token::KEY_RIGHTARROW } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "UPARROW" );
    e = std::vector<Token>( { Token::KEY_UPARROW } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "DOWNARROW" );
    e = std::vector<Token>( { Token::KEY_DOWNARROW } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "KPSLASH" );
    e = std::vector<Token>( { Token::KEY_KPSLASH } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "KPSTAR" );
    e = std::vector<Token>( { Token::KEY_KPSTAR } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "KPMINUS" );
    e = std::vector<Token>( { Token::KEY_KPMINUS } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "KPPLUS" );
    e = std::vector<Token>( { Token::KEY_KPPLUS } );
    QVERIFY( t == e );
}

void ParserTest::abortedCapitalLiterals()
{
    auto t = ParseKeyboardInputsToTokens( "aKPPL" );
    auto e = std::vector<Token>( { Token::KEY_a } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "aKPPLasd" );
    e = std::vector<Token>( { Token::KEY_a } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "aDOESNTEXIST" );
    e = std::vector<Token>( { Token::KEY_a } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "aBACKSPACEBACK" );
    e = std::vector<Token>( { Token::KEY_a, Token::KEY_BACKSPACE } );
    QVERIFY( t == e );
}

void ParserTest::multipleCapitalLiterals()
{
    auto t = ParseKeyboardInputsToTokens( "BACKSPACEa" );
    auto e = std::vector<Token>( { Token::KEY_BACKSPACE, Token::KEY_a } );
    QCOMPARE( t, e );

    t = ParseKeyboardInputsToTokens( "BACKSPACEBACKSPACE" );
    e = std::vector<Token>( { Token::KEY_BACKSPACE, Token::KEY_BACKSPACE } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "BACKSPACEaBACKSPACE" );
    e = std::vector<Token>(
        { Token::KEY_BACKSPACE, Token::KEY_a, Token::KEY_BACKSPACE } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F1BACKSPACEaBACKSPACE" );
    e = std::vector<Token>( { Token::KEY_F1,
                              Token::KEY_BACKSPACE,
                              Token::KEY_a,
                              Token::KEY_BACKSPACE } );
    QVERIFY( t == e );

    t = ParseKeyboardInputsToTokens( "F1BACKSPACEaBACKSPACe" );
    e = std::vector<Token>(
        { Token::KEY_F1, Token::KEY_BACKSPACE, Token::KEY_a } );
    QVERIFY( t == e );
}

void ParserTest::sixtyfourBackspacesBenchmarked()
{
    constexpr auto sixtyfourBackspaces
        = "BACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKS"
          "PACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEB"
          "ACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSP"
          "ACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBA"
          "CKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPA"
          "CEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBAC"
          "KSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPAC"
          "EBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACKSPACEBACK"
          "SPACEBACKSPACEBACKSPACEBACKSPACE";

    auto t = ParseKeyboardInputsToTokens( sixtyfourBackspaces );
    std::vector<Token> e( 64, Token::KEY_BACKSPACE );
    QVERIFY( t == e );

    QBENCHMARK
    {
        auto t = ParseKeyboardInputsToTokens( sixtyfourBackspaces );
    }
}

void ParserTest::sixtyfourAsBenchmarked()
{
    constexpr auto sixtyfourBackspaces
        = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    auto t = ParseKeyboardInputsToTokens( sixtyfourBackspaces );
    std::vector<Token> e( 64, Token::KEY_a );
    QVERIFY( t == e );

    QBENCHMARK
    {
        auto t = ParseKeyboardInputsToTokens( sixtyfourBackspaces );
    }
}

void ParserTest::removeDuplicateSpaces()
{
    const auto e = std::vector<Token>( { Token::KEY_a,
                                         Token::TOKEN_NEW_SEQUENCE,
                                         Token::TOKEN_NEW_SEQUENCE,
                                         Token::TOKEN_NEW_SEQUENCE,
                                         Token::TOKEN_NEW_SEQUENCE,
                                         Token::TOKEN_NEW_SEQUENCE,
                                         Token::TOKEN_NEW_SEQUENCE,
                                         Token::KEY_d } );
    const auto t = std::vector<Token>(
        { Token::KEY_a, Token::TOKEN_NEW_SEQUENCE, Token::KEY_d } );

    QCOMPARE( removeIncorrectTokens( e ), t );
}

void ParserTest::removeDuplicateModifiers()
{
    auto e = std::vector<Token>( { Token::KEY_a,
                                   Token::MODIFIER_CTRL,
                                   Token::KEY_a,
                                   Token::MODIFIER_CTRL,
                                   Token::TOKEN_NEW_SEQUENCE,
                                   Token::MODIFIER_ALT,
                                   Token::KEY_TAB,
                                   Token::KEY_d } );
    auto t = std::vector<Token>( { Token::KEY_a,
                                   Token::MODIFIER_CTRL,
                                   Token::KEY_a,
                                   Token::TOKEN_NEW_SEQUENCE,
                                   Token::MODIFIER_ALT,
                                   Token::KEY_TAB,
                                   Token::KEY_d } );

    QCOMPARE( removeIncorrectTokens( e ), t );

    e = std::vector<Token>( { Token::KEY_a,
                              Token::MODIFIER_ALT,
                              Token::KEY_a,
                              Token::MODIFIER_ALT,
                              Token::TOKEN_NEW_SEQUENCE,
                              Token::MODIFIER_ALT,
                              Token::KEY_TAB,
                              Token::KEY_d } );
    t = std::vector<Token>( { Token::KEY_a,
                              Token::MODIFIER_ALT,
                              Token::KEY_a,
                              Token::TOKEN_NEW_SEQUENCE,
                              Token::MODIFIER_ALT,
                              Token::KEY_TAB,
                              Token::KEY_d } );

    QCOMPARE( removeIncorrectTokens( e ), t );

    e = std::vector<Token>( { Token::KEY_a,
                              Token::MODIFIER_SHIFT,
                              Token::KEY_a,
                              Token::MODIFIER_SHIFT,
                              Token::TOKEN_NEW_SEQUENCE,
                              Token::MODIFIER_ALT,
                              Token::KEY_TAB,
                              Token::KEY_d } );
    t = std::vector<Token>( { Token::KEY_a,
                              Token::MODIFIER_SHIFT,
                              Token::KEY_a,
                              Token::TOKEN_NEW_SEQUENCE,
                              Token::MODIFIER_ALT,
                              Token::KEY_TAB,
                              Token::KEY_d } );

    QCOMPARE( removeIncorrectTokens( e ), t );

    e = std::vector<Token>( { Token::KEY_a,
                              Token::MODIFIER_SUPER,
                              Token::KEY_a,
                              Token::MODIFIER_SUPER,
                              Token::TOKEN_NEW_SEQUENCE,
                              Token::MODIFIER_ALT,
                              Token::KEY_TAB,
                              Token::KEY_d } );
    t = std::vector<Token>( { Token::KEY_a,
                              Token::MODIFIER_SUPER,
                              Token::KEY_a,
                              Token::TOKEN_NEW_SEQUENCE,
                              Token::MODIFIER_ALT,
                              Token::KEY_TAB,
                              Token::KEY_d } );

    QCOMPARE( removeIncorrectTokens( e ), t );
}

void ParserTest::removeIncorrectTokensBenchmark()
{
    auto e = std::vector<Token>( 100, Token::MODIFIER_SHIFT );

    e.at( 0 ) = Token::KEY_a;
    e.at( 10 ) = Token::TOKEN_NEW_SEQUENCE;
    e.at( 11 ) = Token::KEY_a;
    e.at( 95 ) = Token::KEY_a;

    auto t = std::vector<Token>( { Token::KEY_a,
                                   Token::MODIFIER_SHIFT,
                                   Token::TOKEN_NEW_SEQUENCE,
                                   Token::KEY_a,
                                   Token::MODIFIER_SHIFT,
                                   Token::KEY_a } );

    QCOMPARE( removeIncorrectTokens( e ), t );

    QBENCHMARK
    {
        QCOMPARE( removeIncorrectTokens( e ), t );
    }
}

QTEST_APPLESS_MAIN( ParserTest )

#include "./release/tst_parsertest.moc"
