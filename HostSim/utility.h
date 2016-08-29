#ifndef UTILITY_H
#define UTILITY_H

class QString;

class Utility{
public:
	static void answer_question (QString *pAnswer, const QString &question);
	static void response_buy (QString *pResponse, const QString &order);

	Utility();
};

#endif // UTILITY_H
