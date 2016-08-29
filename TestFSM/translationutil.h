#ifndef TRANSLATIONUTIL_H
#define TRANSLATIONUTIL_H

#include <QtGui>

class QObject;
class QString;
class QTranslator;

class TranslationUtil : public QObject{
	Q_OBJECT
	Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)
public:
	TranslationUtil();

	QString getEmptyString() {
		return "";
	}
	Q_INVOKABLE void setLanguage(QString language);

signals:
	void languageChanged();

private:
	QTranslator *m_pTranslator_de;
	QTranslator *m_pTranslator_it;
};

#endif // TRANSLATIONUTIL_H

