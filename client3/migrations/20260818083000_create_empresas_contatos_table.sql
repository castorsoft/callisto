-- Table: public.empresa_contatos

-- DROP TABLE IF EXISTS public.empresa_contatos;

CREATE TABLE IF NOT EXISTS public.empresa_contatos
(
    id SERIAL,
    id_empresa integer NOT NULL,
    id_contato integer NOT NULL,
    CONSTRAINT empresa_contatos_pkey PRIMARY KEY (id, id_contato),
    CONSTRAINT fk_empresa_contatos_contato FOREIGN KEY (id_contato)
        REFERENCES public.contatos (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT fk_empresa_contatos_empresa FOREIGN KEY (id_empresa)
        REFERENCES public.empresas (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;