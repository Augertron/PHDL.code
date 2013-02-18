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

" Comments
syntax match phdlTODO "TODO\|FIXME" contained
syntax match phdlLineComment "//.*" contains=phdlTODO
syntax region phdlBlockComment start="/\*" end="\*/" contains=phdlBlockComment,phdlTODO

" Attributes
syntax keyword phdlKeyword attribute
execute "syntax match phdlAttribute \"@" . s:NAME . "\""

" Parameters
syntax keyword phdlKeyword parameter
execute "syntax match phdlParameter \"$" . s:NAME . "\""
"
" Nets, Ports, and Pins
syntax keyword phdlKeyword net port pin
"
" Instances
syntax keyword phdlKeyword instance of
"
" Packages, Designs, and Devices
syntax keyword phdlDefinition package device design
"
" Imports
syntax keyword phdlImport import
"
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

" Names
syntax match phdlError /\\./
syntax match phdlNameError /\\[^"\\]/ contained
syntax match phdlNameEscape /\\[\\"]/ contained
syntax match phdlNameText /[^\\"]\+/ contained
syntax match phdlNameEscapeNumber /[[:xdigit:]]\+/ contained
syntax match phdlNameEscape /\\x{[[:xdigit:]]\+}/ contains=phdlNameEscapeNumber contained
syntax region phdlName start=/"/ skip=/\\"/ end=/"/ contains=phdlNameError,phdlNameEscape,phdlNameText

highlight default link phdlAttribute String
highlight default link phdlBlockComment Comment
highlight default link phdlBracket Delimiter
highlight default link phdlDefinition Function
highlight default link phdlError Error
highlight default link phdlImport PreProc
highlight default link phdlKeyword Keyword
highlight default link phdlLineComment Comment
highlight default link phdlName SpecialChar
highlight default link phdlNameText Normal
highlight default link phdlNameError Error
highlight default link phdlNameEscape SpecialChar
highlight default link phdlNameEscapeNumber Number
highlight default link phdlNameQuotes SpecialChar
highlight default link phdlParameter Identifier
highlight default link phdlSlice Special
highlight default link phdlSliceNumber Number
highlight default link phdlSymbol Operator
highlight default link phdlTODO Todo
