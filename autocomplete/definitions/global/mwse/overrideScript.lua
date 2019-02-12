return {
	type = "function",
	description = [[Configures MWSE to execute a given function instead when a script would run.]],
	arguments = {
		{ name = "scriptId", type = "string" },
		{ name = "callback", type = "function" },
	},
	returns = "boolean",
}