-- Table: public.produtos_alteracoes

-- DROP TABLE IF EXISTS public.produtos_alteracoes;

CREATE TABLE IF NOT EXISTS public.produtos_alteracoes
(
    id SERIAL,
    id_produto integer NOT NULL,
    tipo tipo_produto_alteracao NOT NULL,
    anterior character varying(120) COLLATE pg_catalog."default",
    novo character varying(120) COLLATE pg_catalog."default",
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT produtos_alteracoes_pkey PRIMARY KEY (id),
    CONSTRAINT fk_produtos_alteracoes_produto FOREIGN KEY (id_produto)
        REFERENCES public.produtos (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;