-- Table: public.cidades

-- DROP TABLE IF EXISTS public.cidades;

CREATE TABLE IF NOT EXISTS public.cidades
(
    id SERIAL,
    nome character varying(120) COLLATE pg_catalog."default" NOT NULL,
    id_ibge integer NOT NULL,
    id_estado integer NOT NULL,
    CONSTRAINT cidades_pk PRIMARY KEY (id),
    CONSTRAINT cidades_unique UNIQUE (id_ibge),
    CONSTRAINT cidades_estados_fk FOREIGN KEY (id_estado)
        REFERENCES public.estados (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;
