-- FUNCTION: public.fn_contatos_bu0()

-- DROP FUNCTION IF EXISTS public.fn_contatos_bu0();

CREATE OR REPLACE FUNCTION public.fn_contatos_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: contatos_bu0

-- DROP TRIGGER IF EXISTS contatos_bu0 ON public.contatos;

CREATE OR REPLACE TRIGGER contatos_bu0
    BEFORE UPDATE
    ON public.contatos
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_contatos_bu0();