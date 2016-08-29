#include "utility.h"
#include <QString>

void Utility::answer_question (QString *pAnswer, const QString &question){
	*pAnswer = "This is the answer";
	int pos = question.lastIndexOf(' ');
	if (pos != -1){
		*pAnswer += " to your question ";
		*pAnswer += question.right(question.size() - pos - 1);
	}
}
void Utility::response_buy (QString *pResponse, const QString &order){
	*pResponse = "Your order successfully placed";
}

Utility::Utility()
{

}
