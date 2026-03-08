CREATE TABLE public.migrations (
	id int4 NOT NULL,
	migration varchar(255) COLLATE "pt-BR-x-icu" NOT NULL,
	CONSTRAINT migrations_pk PRIMARY KEY (id)
);

CREATE SEQUENCE migrations_seq;

CREATE OR REPLACE FUNCTION public.migrations_bi0_fn()
	RETURNS trigger
AS $function$
	BEGIN
		if (new.id is null) then
			new.id = nextval('migrations_seq');
		end if;
    RETURN NEW;
	END;
$function$ LANGUAGE plpgsql;

CREATE TRIGGER migrations_bi0 BEFORE INSERT ON public.migrations FOR EACH ROW EXECUTE FUNCTION migrations_bi0_fn();