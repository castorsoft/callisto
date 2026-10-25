-- FUNCTION: public.fn_enderecos_bu0()

-- DROP FUNCTION IF EXISTS public.fn_enderecos_bu0();

CREATE OR REPLACE FUNCTION public.fn_enderecos_bu0()
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

-- Trigger: enderecos_bu0

-- DROP TRIGGER IF EXISTS enderecos_bu0 ON public.enderecos;

CREATE OR REPLACE TRIGGER enderecos_bu0
    BEFORE UPDATE
    ON public.enderecos
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_enderecos_bu0();