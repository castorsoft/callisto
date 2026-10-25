-- Table: public.pessoas_alteracoes

-- DROP TABLE IF EXISTS public.pessoas_alteracoes;

CREATE TABLE IF NOT EXISTS public.pessoas_alteracoes
(
    id SERIAL,
    id_pessoa integer NOT NULL,
    tipo tipo_pessoa_alteracao NOT NULL,
    anterior character varying(100) COLLATE pg_catalog."default",
    novo character varying(100) COLLATE pg_catalog."default",
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT pessoas_alteracoes_pkey PRIMARY KEY (id, id_pessoa),
    CONSTRAINT fk_pessoas_alteracoes_pessoas FOREIGN KEY (id_pessoa)
        REFERENCES public.pessoas (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;