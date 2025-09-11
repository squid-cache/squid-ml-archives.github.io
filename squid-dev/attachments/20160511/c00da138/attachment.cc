/// obs-fold = CRLF RWS
/// our-obs-fold = *relaxed_whitespace LF RWS
/// We allow relaxed delimiters before obs-fold, which takes care of CR and
/// various garbage whitespace that an obs-fold-sending agent may send.
/// Like Tokenizer.skip(our-obs-fold).
static bool
skipObsfold(Tokenizer &tk)
{
    Tokenizer testing(tk);
    (void)testing.skipAll(RelaxedDelimiterCharacters());
    if (testing.skip(CharacterSet::LF) && testing.skipAll(CharacterSet::WSP)) {
        tk = testing;
        return true;
    }
    return false;
}

/// obs-folds = 1*obs-fold
/// Like Tokenizer.skipAll(obs-fold).
static bool
skipAllObsFolds(Tokenizer &tk)
{
    bool found = false;
    while (skipObsfold(tk))
        found = true;
    return found;
}

/// delimiter = obs-folds
/// Like Tokenizer.token(), skips everything up to the first set of delimiters.
/// Upon success, sets prefix to characters before the first found delimiter.
static bool
tokenBeforeObsfolds(Tokenizer &tk, SBuf &prefix)
{
    Tokenizer testing(tk);
    static const CharacterSet nonLF = CharacterSet::LF.complement().rename("non-LF");
    if (testing.token(prefix, nonLF) && skipAllObsFolds(testing)) {
        Tokenizer trimming(prefix);
        trimming.skipAllTrailing(RelaxedDelimiterCharacters);
        prefix = trimming.remaining();
        tk = testing;
        return true;
    }
    return false;
}

void
Http::One::Parser::unfoldMime()
{
    Tokenizer tok(mimeHeaderBlock_);
    mimeHeaderBlock_.clear();
    SBuf prefix;
    while (tokenBeforeObsfolds(tk, prefix)) {
        mimeHeaderBlock_.append(prefix);
        mimeHeaderBlock_.append(' ');
    }
    mimeHeaderBlock_.append(tk.remaining());
}
