
void token_test()
{
	const char* test_data = "a123=2  +3*4*8;";
	TokenParser parser;
	int token = parser.Parse(test_data, strlen(test_data));
	while(token != TOKEN_EOF)
	{
		printf("tokden type: %d", token);
		char buf[256];
		int len = sizeof(buf);
		switch(token)
		{
		case TOKEN_NUM:
		case TOKEN_IDENT:
		case TOKEN_STR:
			parser.GetTokenValue(buf, len);
			printf(", value: %s", buf);
			break;
		}
		printf("\n");
		token = parser.Parse();
	};
}