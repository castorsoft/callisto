-- Table: public.contatos

-- DROP TABLE IF EXISTS public.contatos;

CREATE TABLE IF NOT EXISTS public.contatos
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    tipo tipo_contato NOT NULL,
    contato character varying(60) COLLATE pg_catalog."default" NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT contatos_pkey PRIMARY KEY (id),
    CONSTRAINT contatos_unique UNIQUE (uuid)
)


TABLESPACE pg_default;