-- Table: public.documentos_fiscais_arquivos

-- DROP TABLE IF EXISTS public.documentos_fiscais_arquivos;

CREATE TABLE IF NOT EXISTS public.documentos_fiscais_arquivos
(
    id SERIAL,
    uuid uuid NOT NULL,
    documento text COLLATE pg_catalog."default" NOT NULL,
    chave_acesso bit varying(50) NOT NULL,
    processado boolean NOT NULL DEFAULT false,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    dados jsonb,
    CONSTRAINT documentos_fiscais_arquivos_pkey PRIMARY KEY (id)
)

TABLESPACE pg_default;