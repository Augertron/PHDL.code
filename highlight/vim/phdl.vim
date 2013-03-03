" This is a PHDL syntax definition file for use by the Vim editor.
" See <http://vimdoc.sourceforge.net/htmldoc/syntax.html> for more info.

" Syntax file boilerplate
if exists("b:current_syntax")
  finish
endif
let b:current_syntax = "phdl"
syntax clear
syntax sync fromstart

" Use case-sensitive matching
syntax case match

"# Syntax variables
let s:NAME = "\\([[:alnum:]_]\\|[^[:print:]]\\)\\+"
let s:TODO = "TODO\\|FIXME"

" Escapes
syntax match phdlError /\\./

" Comments
syntax match phdlTODO "TODO\|FIXME" contained
syntax match phdlLineComment "//.*" contains=phdlTODO
syntax region phdlBlockComment start="/\*" end="\*/" contains=phdlBlockComment,phdlTODO

" Nets, Ports, and Pins
syntax keyword phdlKeyword net port pin

" Instances
syntax keyword phdlKeyword instance of

" Packages, Designs, and Devices
syntax keyword phdlKeyword package device design is

" Imports
syntax keyword phdlImport import

" Slices
syntax match phdlSliceNumber "[[:digit:]+]" contained
syntax region phdlSlice start="\[" end="\]" contains=phdlSliceNumber

" Expansions
syntax match phdlSlice "[<>]"

" Concatenation
syntax match phdlSlice ","

" Brackets
syntax match phdlBracket "[{}]"

" Assignment
syntax match phdlSymbol "="

" Qualification
syntax match phdlSymbol "\."

" End-of-Statement
syntax match phdlSymbol "\;"

" Quoted Subrules
syntax match phdlQuotedError /\\[^"\\]/ contains=NONE contained
syntax match phdlQuotedEscape /\\[\\"]/ contains=NONE contained
syntax match phdlQuotedEscapeNumber /[[:xdigit:]]\+/ contains=NONE contained
syntax match phdlQuotedEscape /\\x{[[:xdigit:]]\+}/ contains=phdlQuotedEscapeNumber contained
syntax match phdlQuotedQuotes /"/ contains=NONE contained
syntax match phdlQuotedText /[^\\"]\+/ transparent contains=NONE contained
syntax cluster phdlQuoted contains=phdlQuotedError,phdlQuotedEscape,phdlQuotedQuotes,phdlQuotedText

" Attributes
syntax keyword phdlKeyword attribute
execute "syntax match phdlAttribute \"@" . s:NAME . "\""
syntax region phdlAttribute start=/@"/ skip=/\\"/ end=/"/ keepend contains=@phdlQuoted

" Parameters
syntax keyword phdlKeyword parameter
execute "syntax match phdlParameter \"$" . s:NAME . "\""
syntax region phdlParameter start=/\$"/ skip=/\\"/ end=/"/ keepend contains=@phdlQuoted

" Names
syntax match phdlNameText /[^\\"]\+/ contained
syntax region phdlName start=/"/ skip=/\\"/ end=/"/ keepend contains=@phdlQuoted

highlight default link phdlAttribute String
highlight default link phdlAttributeText String
highlight default link phdlBlockComment Comment
highlight default link phdlBracket Delimiter
highlight default link phdlError Error
highlight default link phdlImport PreProc
highlight default link phdlKeyword Keyword
highlight default link phdlLineComment Comment
highlight default link phdlName Normal
highlight default link phdlNameText Normal
highlight default link phdlParameter Identifier
highlight default link phdlParameterText Identifier
highlight default link phdlQuotedError Error
highlight default link phdlQuotedEscape SpecialChar
highlight default link phdlQuotedEscapeNumber Number
highlight default link phdlQuotedQuotes SpecialChar
highlight default link phdlSlice Special
highlight default link phdlSliceNumber Number
highlight default link phdlSymbol Operator
highlight default link phdlTODO Todo
