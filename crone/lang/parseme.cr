
last term before the block decides how the block will be parsed;

for example, in "verbatim { block }", the parser checks for a "verbatim" subparser and, if it exists, uses it.

in "trimmed verbatim {block}", the verbatim subparser could perhaps check for modifiers such as "trimmed".

parse priority (lowest first)

3 2 1 0 { 4 }

parse-time-meaningless terms may have interpret-time meanings.

there are many "times":

parse time - execute while parsing
interpret time - execute after everything is parsed
phoenix time - execute after having already run everything else & restarted, assuming everything has run successfully


term0 term1      term2 term3 {
    block block

    block {
        blooooooock!!!!
        block!!!;

        block! !!! {
            INNERMOST BLOCK;
        }
    }

    block
}

term4 term5;

v{verbatim syntax?}

verbatim{alternate option}

trimmed v{ lets you put some spaces on the edges, for readability }

hex FF0000;

should i [add] alternate (forms) of "differentiated" {blocks} <and> punctuation?

{

    no reason you {can't} keep writing after {a block};

    #=================================================#

    but ideally the {
        linter
    }

    would format it {
        like this
    }

}

extraneous punctuation;;;

sob ;_;


here are some more terms { and a block }

and some more terms with no block;

some terms
    with whitespace;


deep nesting {
    deeep nesting {
        deeeep nesting {
            asdf {
                asdff {
                    asdfff {
                        foo {
                            bar {
                                baz {
                                    qwertyuiopasdfghjklzxcvbnm {
                                        ;
                                    }
                                }
                            }
                        }
                    }
                }

                asdfasdf {
                    asdfasdfasdf asdf {

                    }
                }
            }
        }
    }
}


let's do some blocks {
    waow;

    this is a block {
        inside a block {
            another block;

            with content;

            including { this block }
        }


        !!!;;;

        !!


        !;
    }
}

