-- Table: public.produtos_tributacoes

-- DROP TABLE IF EXISTS public.produtos_tributacoes;

CREATE TABLE IF NOT EXISTS public.produtos_tributacoes
(
    id SERIAL,
    id_produto integer NOT NULL,
    tipo_tributo tipo_produto_tributacao NOT NULL,
    dados jsonb,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT produtos_tributacoes_pkey PRIMARY KEY (id, id_produto),
    CONSTRAINT unq_produtos_tributacao_tipo UNIQUE (id_produto, tipo_tributo),
    CONSTRAINT fk_produto_tributacoes_produto FOREIGN KEY (id_produto)
        REFERENCES public.produtos (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;