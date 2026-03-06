CREATE TABLE public.migrations (
	id int4 NOT NULL,
	migration varchar(255) COLLATE "pt-BR-x-icu" NOT NULL,
	batch int4 NOT NULL,
	CONSTRAINT migrations_pk PRIMARY KEY (id)
);