CREATE TABLE public.migrations (
	id SERIAL,
	migration varchar(255) COLLATE "pt-BR-x-icu" NOT NULL,
	CONSTRAINT migrations_pk PRIMARY KEY (id)
);