#include "translationutil.h"

void TranslationUtil::setLanguage(QString language) {
	if(language == QString("de")) {
		if (m_pTranslator_de->load(":/texts/texts/sbmain_de")){
			qApp->installTranslator(m_pTranslator_de);
		}
	} else if(language == QString("it")) {
		if (m_pTranslator_it->load(":/texts/texts/sbmain_it")){
			qApp->installTranslator(m_pTranslator_it);
		}
	} else {
		qApp->removeTranslator(m_pTranslator_de);
		qApp->removeTranslator(m_pTranslator_it);
	}
	emit languageChanged();
}

TranslationUtil::TranslationUtil()
: m_pTranslator_it	(NULL)
, m_pTranslator_de	(NULL)
{
	m_pTranslator_it = new QTranslator(this);
	m_pTranslator_de = new QTranslator(this);
}

