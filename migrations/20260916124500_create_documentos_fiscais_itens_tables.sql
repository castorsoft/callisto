-- Table: public.documentos_fiscais_itens

-- DROP TABLE IF EXISTS public.documentos_fiscais_itens;

CREATE TABLE IF NOT EXISTS public.documentos_fiscais_itens
(
    id SERIAL PRIMARY KEY,
    id_documento_fiscal integer NOT NULL,
    numero_item integer NOT NULL,
    codigo_produto character varying(60) COLLATE pg_catalog."default" NOT NULL,
    descricao character varying(120) COLLATE pg_catalog."default" NOT NULL,
    unidade_medida character varying(10) COLLATE pg_catalog."default",
    quantidade numeric(15,4) NOT NULL,
    valor_unitario numeric(15,4) NOT NULL,
    valor_total numeric(15,4) NOT NULL,
    valor_desconto numeric(15,4) DEFAULT 0.0000,
    dados jsonb,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_doc_fiscal_itens_doc FOREIGN KEY (id_documento_fiscal)
        REFERENCES public.documentos_fiscais (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE CASCADE
)
TABLESPACE pg_default;

-- Table: public.documentos_fiscais_itens_tributacoes

-- DROP TABLE IF EXISTS public.documentos_fiscais_itens_tributacoes;

CREATE TABLE IF NOT EXISTS public.documentos_fiscais_itens_tributacoes
(
    id SERIAL PRIMARY KEY,
    id_item integer NOT NULL,
    tipo_tributo tipo_produto_tributacao NOT NULL,
    dados jsonb,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT unq_doc_fiscais_itens_tributacao_tipo UNIQUE (id_item, tipo_tributo),
    CONSTRAINT fk_doc_fiscais_itens_tributacoes_item FOREIGN KEY (id_item)
        REFERENCES public.documentos_fiscais_itens (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE CASCADE
)
TABLESPACE pg_default;
