-- Table: public.pessoas_fornecedor

-- DROP TABLE IF EXISTS public.pessoas_fornecedor;

CREATE TABLE IF NOT EXISTS public.pessoas_fornecedor
(
    id SERIAL,
    id_pessoa integer NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT pessoas_fornecedor_pkey PRIMARY KEY (id),
    CONSTRAINT fk_pessoa_fornecedor_pessoa FOREIGN KEY (id_pessoa)
        REFERENCES public.pessoas (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;