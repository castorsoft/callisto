#include "Arquivo.h"

#include <QFileInfo>

Arquivo Arquivo::loadFromDatabase(int id, QUuid uuid, QString caminhoArquivo, QString nomeArquivo, bool processado, bool possuiErro, std::optional<QString> mensagemErro, std::optional<bool> processarNovamente, QDateTime criadoEm, QDateTime atualizadoEm)
{
    return Arquivo(id, uuid, caminhoArquivo, nomeArquivo, processado, possuiErro, mensagemErro, processarNovamente, criadoEm, atualizadoEm);
}

void Arquivo::definirArquivo(QString caminhoArquivo)
{
    QFileInfo arquivo(caminhoArquivo);
    m_nomeArquivo = arquivo.fileName();
    m_caminhoArquivo = arquivo.absoluteFilePath();
}

void Arquivo::definirComoProcessado()
{
    m_processado = true;
}

void Arquivo::definirComErro(QString mensagemErro, bool processarNovamente)
{
    m_possuiErro = true;
    m_mensagemErro = mensagemErro;
    m_processarNovamente = processarNovamente;
}

void Arquivo::hidratar(int id, QUuid uuid, QDateTime criadoEm, QDateTime atualizadoEm)
{
    m_id = id;
    m_uuid = uuid;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}

void Arquivo::hidratar(QDateTime atualizadoEm)
{
    m_atualizadoEm = atualizadoEm;
}
