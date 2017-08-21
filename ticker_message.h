#ifndef _TICKER_MESSAGE_H_
#define _TICKER_MESSAGE_H_

class TickerMessage {
	public:
		std::string message;
		std::string fontName;
		int yAreaSize;
		TickerMessage(std::string m, std::string f) {
			message = m;
			fontName = f;
			yAreaSize = 11;
		}
};

#endif // _TICKER_MESSAGE_H_
