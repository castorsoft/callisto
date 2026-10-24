#pragma once

#include <QString>
#include <QDateTime>
#include <QSqlRecord>
#include <QMetaType>

class NotaTerceiroResumo {
public:
    NotaTerceiroResumo() = default;

    static NotaTerceiroResumo fromRecord(const QSqlRecord &record, const QString &prefix = "");

    int idDocumentoFiscal() const { return m_idDocumentoFiscal; }
    void setIdDocumentoFiscal(int id) { m_idDocumentoFiscal = id; }

    QString chaveAcesso() const { return m_chaveAcesso; }
    void setChaveAcesso(const QString &chave) { m_chaveAcesso = chave; }

    QString modelo() const { return m_modelo; }
    void setModelo(const QString &modelo) { m_modelo = modelo; }

    QString serie() const { return m_serie; }
    void setSerie(const QString &serie) { m_serie = serie; }

    int numero() const { return m_numero; }
    void setNumero(int numero) { m_numero = numero; }

    QDateTime dataEmissao() const { return m_dataEmissao; }
    void setDataEmissao(const QDateTime &data) { m_dataEmissao = data; }

    int idFornecedor() const { return m_idFornecedor; }
    void setIdFornecedor(int id) { m_idFornecedor = id; }

    QString fornecedorNome() const { return m_fornecedorNome; }
    void setFornecedorNome(const QString &nome) { m_fornecedorNome = nome; }

    QString fornecedorCnpj() const { return m_fornecedorCnpj; }
    void setFornecedorCnpj(const QString &cnpj) { m_fornecedorCnpj = cnpj; }

    int totalItens() const { return m_totalItens; }
    void setTotalItens(int total) { m_totalItens = total; }

    int itensPendentes() const { return m_itensPendentes; }
    void setItensPendentes(int pendentes) { m_itensPendentes = pendentes; }

private:
    int m_idDocumentoFiscal{0};
    QString m_chaveAcesso;
    QString m_modelo;
    QString m_serie;
    int m_numero{0};
    QDateTime m_dataEmissao;
    int m_idFornecedor{0};
    QString m_fornecedorNome;
    QString m_fornecedorCnpj;
    int m_totalItens{0};
    int m_itensPendentes{0};
};

Q_DECLARE_METATYPE(NotaTerceiroResumo)
