-- Table: public.documentos_fiscais

-- DROP TABLE IF EXISTS public.documentos_fiscais;

CREATE TABLE IF NOT EXISTS public.documentos_fiscais
(
    id SERIAL,
    chave_acesso character varying(50) COLLATE pg_catalog."default" NOT NULL,
    tipo_operacao tipo_dfe_operacao NOT NULL,
    tipo_emissao tipo_dfe_emissao NOT NULL,
    id_pessoa integer NOT NULL,
    modelo character varying(5) COLLATE pg_catalog."default" NOT NULL,
    serie character varying(5) COLLATE pg_catalog."default",
    numero integer NOT NULL,
    dh_emissao timestamp without time zone NOT NULL,
    dados jsonb,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT documentos_fiscais_pkey PRIMARY KEY (id, id_pessoa)
)

TABLESPACE pg_default;