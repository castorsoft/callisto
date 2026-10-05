#include "Arquivo.h"
#include <QFileInfo>
#include <QVariant>

Arquivo Arquivo::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    Arquivo a;
    
    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        a.setId(record.value(prefix + "id").toInt());
        
    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        a.setUuid(record.value(prefix + "uuid").toUuid());
        
    if (record.contains(prefix + "caminho_arquivo"))
        a.m_caminhoArquivo = record.value(prefix + "caminho_arquivo").toString();
        
    if (record.contains(prefix + "nome_arquivo"))
        a.m_nomeArquivo = record.value(prefix + "nome_arquivo").toString();
        
    if (record.contains(prefix + "processado"))
        a.m_processado = record.value(prefix + "processado").toBool();
        
    if (record.contains(prefix + "possui_erro"))
        a.m_possuiErro = record.value(prefix + "possui_erro").toBool();
        
    if (record.contains(prefix + "mensagem_erro"))
        a.m_mensagemErro = record.isNull(prefix + "mensagem_erro") ? std::nullopt : std::make_optional(record.value(prefix + "mensagem_erro").toString());
        
    if (record.contains(prefix + "processar_novamente"))
        a.m_processarNovamente = record.isNull(prefix + "processar_novamente") ? std::nullopt : std::make_optional(record.value(prefix + "processar_novamente").toBool());
        
    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        a.setCriadoEm(record.value(prefix + "criado_em").toDateTime());
        
    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        a.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return a;
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
