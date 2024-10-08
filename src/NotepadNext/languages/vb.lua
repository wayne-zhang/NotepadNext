local L = {}

L.lexer = "vb"

L.singleLineComment = "' "

L.extensions = {
	"vb",
	"vbs",
}

L.keywords = {
	[0] = "addhandler addressof alias and andalso as boolean by byref byte byval call case catch cbool cbyte cchar cdate cdbl cdec char cint class clng cobj const continue csbyte cshort csng cstr ctype cuint culng cushort date decimal declare default delegate dim directcast do double each else elseif end endif enum erase error event exit false finally for friend function get gettype global gosub goto handles if implements imports in inherits integer interface is isnot let lib like long loop me mod module mustinherit mustoverride mybase myclass namespace narrowing new next not nothing notinheritable notoverridable object of on operator option optional or orelse out overloads overridable overrides paramarray partial private property protected public raiseevent readonly redim rem removehandler resume return sbyte select set shadows shared short single static step stop strict string structure sub synclock then throw to true try trycast typeof uinteger ulong ushort using variant wend when while widening with withevents writeonly xor attribute begin currency implement load lset rset type unload aggregate ansi assembly async auto await binary compare custom distinct equals explicit from group into isfalse istrue iterator join key mid off order preserve skip take text unicode until where yield",
}

L.styles = {
	["DEFAULT"] = {
		id = 7,
		fgColor = rgb(defaultFg),
		bgColor = rgb(defaultBg),
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x008000),
		bgColor = rgb(defaultBg),
	},
	["NUMBER"] = {
		id = 2,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(defaultBg),
		fontStyle = 1,
	},
	["WORD"] = {
		id = 3,
		fgColor = rgb(InstructionColor),
		bgColor = rgb(defaultBg),
	},
	["STRING"] = {
		id = 4,
		fgColor = rgb(0x808080),
		bgColor = rgb(defaultBg),
	},
	["PREPROCESSOR"] = {
		id = 5,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(defaultBg),
	},
	["OPERATOR"] = {
		id = 6,
		fgColor = rgb(defaultFg),
		bgColor = rgb(defaultBg),
		fontStyle = 1,
	},
	["DATE"] = {
		id = 8,
		fgColor = rgb(0x00FF00),
		bgColor = rgb(defaultBg),
	},
}
return L
