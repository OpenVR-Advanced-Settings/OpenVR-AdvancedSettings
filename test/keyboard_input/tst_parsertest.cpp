#include <QtTest>
#include <QDebug>
#include "input_parser.h"

// add necessary includes here

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

    t = ParseKeyboardInputsToTokens( "/" );
    e = std::vector<Token>( { Token::MODIFIER_ALTGR } );
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

QTEST_APPLESS_MAIN( ParserTest )

#include "tst_parsertest.moc"
